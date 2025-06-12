#include <windows.h>
#include <iostream>

#define global_variable static
#define internal static

bool open = false;
global_variable bool running = true;
global_variable bool runGamrunGamee = false;

struct RenderState {
    int height, width;
    void* memory;

    BITMAPINFO bitmapinfo;
};

global_variable RenderState renderState;
#include "platformCommon.cpp"
#include "renderer.cpp"
#include "Simulate Game.cpp"


LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;


    switch (uMsg) {
        case WM_CLOSE:
        case WM_DESTROY: {
            running = false;
        } break;

        case WM_SIZE: {
            RECT rect;
            GetClientRect(hwnd, &rect);
            renderState.width = rect.right - rect.left;
            renderState.height = rect.bottom - rect.top;

            int bufferSize = renderState.width * renderState.height * sizeof(u32);

            if (renderState.memory) { VirtualFree(renderState.memory, 0, MEM_RELEASE); }
            renderState.memory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

            renderState.bitmapinfo.bmiHeader.biSize = sizeof(renderState.bitmapinfo.bmiHeader);
            renderState.bitmapinfo.bmiHeader.biWidth = renderState.width;
            renderState.bitmapinfo.bmiHeader.biHeight = renderState.height;
            renderState.bitmapinfo.bmiHeader.biPlanes = 1;
            renderState.bitmapinfo.bmiHeader.biBitCount = 32;
            renderState.bitmapinfo.bmiHeader.biCompression = BI_RGB;
        }break;

        default: {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    //Create Window class
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = "Game Window Class" ;
    window_class.lpfnWndProc = window_callback;

    //Register Class
    RegisterClass(&window_class);
    //Create Window
    HWND window = CreateWindow(window_class.lpszClassName, "Pokemon??", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1920, 1080, 0,0 , hInstance, 0);
    HDC hdc = GetDC(window);

    Input input = {};

    float deltaT = 0.016666f;
    LARGE_INTEGER frameBeginTime;
    QueryPerformanceCounter(&frameBeginTime);

    float performanceFreq;
    {
        LARGE_INTEGER perf;
        QueryPerformanceFrequency(&perf);
        performanceFreq = (float)perf.QuadPart;
    }

    while (running) {
        //Input
        MSG message;

        for (int i = 0; i<BUTTON_COUNT; i++) {
            input.buttons[i].changed = false;
        }

        while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {

            switch (message.message) {
                case WM_KEYUP:
                case WM_KEYDOWN: {
                    u32 vk_code = (u32)message.wParam;
                    bool isDown = ((message.lParam & (1 << 31)) == 0);

#define processButton(b, vk)\
case vk: {\
input.buttons[b].changed = isDown != input.buttons[b].isDown;\
input.buttons[b].isDown = isDown;\
}break;
                    switch (vk_code) {
                        processButton(BUTTON_4, VK_NUMPAD4);
                        processButton(BUTTON_5, VK_NUMPAD5);
                        processButton(BUTTON_6, VK_NUMPAD6);
                        processButton(BUTTON_7, VK_NUMPAD7);
                        processButton(BUTTON_8, VK_NUMPAD8);
                        processButton(BUTTON_9, VK_NUMPAD9);
                        processButton(BUTTON_RETURN, VK_RETURN);
                        processButton(BUTTON_Q, 0x51);
                        processButton(BUTTON_W, 0x57);
                        processButton(BUTTON_E, 0x45);
                        processButton(BUTTON_A, 0x41);
                        processButton(BUTTON_S, 0x53);
                        processButton(BUTTON_D, 0x44);
                        processButton(BUTTON_TAB, VK_TAB);


                    }
                }break;

                default: {
                    TranslateMessage(&message);
                    DispatchMessage(&message);
                }
            }

        }
        //Simulate
        clearScreen(0);
	if(runGame) {
		//game
       	simulateGame(&input);
	} else {
		//menu
	}
	



        //renderBackground();
        //Render
        StretchDIBits(hdc, 0, 0, renderState.width, renderState.height, 0, 0, renderState.width, renderState.height, renderState.memory, &renderState.bitmapinfo, DIB_RGB_COLORS, SRCCOPY);

        LARGE_INTEGER frameEndTime;
        QueryPerformanceCounter(&frameEndTime);
        frameBeginTime = frameEndTime;
    }
    return 0;
}

