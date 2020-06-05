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

void drawInclinometer(signed short x, signed short y) {
    signed short x_norm = y/250;
    signed short y_norm = x/1063;
    unsigned char counter = 0;
    if (x_norm < 0) {
        x_norm = -x_norm;
        for (counter; counter < x_norm; counter++) {
            drawChar(counter + 64, 16, '.');
        }
    }
    else {
        for (counter; counter < x_norm; counter++) {
            drawChar(64 - counter, 16, '.');
        }
    }
    if (y_norm < 0) {
        counter = 0;
        y_norm = -y_norm;
        for (counter; counter < y_norm; counter++) {
            drawChar(64, 16 - counter, '.');
        }
    }
    else {
        counter = 0;
        for (counter; counter < y_norm; counter++) {
            drawChar(64, counter + 16, '.');
        }
    }
}

