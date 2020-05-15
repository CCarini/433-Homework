#include "font.h"
#include "ssd1306.h"

void drawChar(unsigned char x, unsigned char y, char c) {
    char ascii_lookup = c - 0x20;
    unsigned char column = 0;
    for (column; column < 5; column++) {
        unsigned char row = 0;
        for (row; row < 8; row++) {
            if (((ASCII[ascii_lookup][column]) >> row) & 1) {
                ssd1306_drawPixel(x + column, y + row, 1);
            }
        }
    }
}

void drawString(unsigned char x, unsigned char y, char* string) {
    unsigned char i = 0;
    unsigned char offset = 0;
    while (string[i] != '\0') {
        drawChar(x + offset,y,string[i]);
        i++;
        offset += 5;
    }
}