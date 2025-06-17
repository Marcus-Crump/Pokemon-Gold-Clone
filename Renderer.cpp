#include "util.cpp"
internal void
clearScreen(unsigned int color) {
    unsigned int* pixel = (unsigned int*)renderState.memory;
        for (int y = 0; y < renderState.height; y++) {
            for (int x = 0; x < renderState.width; x++) {
                *pixel++ = color;
            }
        }
}

internal void
drawRectinPixels ( int x0, int y0, int x1, int y1, u32 color) {
    x0 = clamp(0, x0, renderState.width);
    x1 = clamp(0, x1, renderState.width);
    y0 = clamp(0, y0, renderState.height);
    y1 = clamp(0, y1, renderState.height);
    for (int y = y0; y < y1; y++) {
        u32* pixel = (unsigned int*)renderState.memory + x0 + y*renderState.width;
        for (int x = x0; x < x1; x++) {
            *pixel++ = color;
        }
    }
}

global_variable float renderScale = 0.01f;
internal void 
drawCell(int x, int y, int scale, u32 color) {
    x *= renderState.height*renderScale;
    y *= renderState.height*renderScale;
    float pxhs = .5*renderState.height*renderScale;
    float halfSizeY = .5*renderState.height*renderScale;

    x += renderState.width / 2.f;
    y += renderState.height/ 2.f;
    //Change to pixels 
    int x0 = x - pxhs;
    int x1 = x + pxhs;
    int y0 = y - pxhs;
    int y1 = y + pxhs;
    drawRectinPixels(x0, y0, x1, y1, color);
}

