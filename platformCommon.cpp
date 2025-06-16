struct ButtonState {
    bool isDown;
    bool changed;
};

enum {
    BUTTON_4,
    BUTTON_5,
    BUTTON_6,
    BUTTON_7,
    BUTTON_8,
    BUTTON_9,
    BUTTON_RETURN,
    BUTTON_W,
    BUTTON_A,
    BUTTON_S,
    BUTTON_D,
    BUTTON_Q,
    BUTTON_E,
    BUTTON_TAB,

    BUTTON_COUNT, //Should be the last item
};

struct Input {
    ButtonState buttons [BUTTON_COUNT];
};
