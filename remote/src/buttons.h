#include <stdint.h>

#define TOUCH 0
#define CLICK 1
#define NEXT  2
#define PREV  3
#define FULL  4
#define ESC   5
#define PLAY  6

struct Button {
    char ch;
    volatile uint8_t pressed;
    uint32_t lastDebounce;

    Button(char _ch): ch(_ch), pressed(0), lastDebounce(0){}
};
