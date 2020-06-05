#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro

#include "ssd1306.h"
#include "font.h"
#include <stdio.h>
#include "adc.h"
#include "ws2812b.h"

// DEVCFG0
#pragma config DEBUG = OFF // disable debugging
#pragma config JTAGEN = OFF // disable jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // disable flash write protect
#pragma config BWP = OFF // disable boot write protect
#pragma config CP = OFF // disable code protect

// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // disable secondary oscillator
#pragma config IESO = OFF // disable switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // disable clock output
#pragma config FPBDIV = DIV_1 // divide sysclk freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // disable clock switch and FSCM
#pragma config WDTPS = PS1048576 // use largest wdt
#pragma config WINDIS = OFF // use non-window mode wdt
#pragma config FWDTEN = OFF // wdt disabled
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%

// DEVCFG2 - get the sysclk clock to 48MHz from the 8MHz crystal
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz
#pragma config FPLLMUL = MUL_24 // multiply clock after FPLLIDIV
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 48MHz

// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = OFF // allow multiple reconfigurations
#pragma config IOL1WAY = OFF // allow multiple reconfigurations

int main() {

    __builtin_disable_interrupts(); // disable interrupts while initializing things

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // do your TRIS and LAT commands here
    i2c_master_setup();
    adc_setup();
    ctmu_setup();
    ws2812b_setup();
            
    __builtin_enable_interrupts();
    
    ssd1306_setup();
    char message[50];
    
    int Captured_Left_Base_Capacitance = 785;
    int Captured_Right_Base_Capacitance = 845;
    
    int Current_Left_Value = 0;
    int Current_Right_Value = 0;
    
    int Delta_Left = 0;
    int Delta_Right = 0;
    
    int Left_Position = 0;
    int Right_Position = 0;
    int Position = 0;
    
    wsColor myColorArray[4];
    
    float brightness = 0;
    
    while (1) {
        int i = 0;
        Current_Left_Value = 0;
        Current_Right_Value = 0;
        for (i; i < 10; i++) {
            Current_Left_Value += ctmu_read(5, 15);
            Current_Right_Value += ctmu_read(4, 15);
        }
        Current_Left_Value = Current_Left_Value/10;
        Current_Right_Value = Current_Right_Value/10;
        if (Current_Left_Value < 500) {
            myColorArray[0] = HSBtoRGB(0,1,.8);
        }
        else {
            myColorArray[0] = HSBtoRGB(0,0,0);
        }
        if (Current_Right_Value < 500) {
            myColorArray[3] = HSBtoRGB(180,1,.8);
        }
        else {
            myColorArray[3] = HSBtoRGB(0,0,0);
        }
        
        Delta_Left = (Captured_Left_Base_Capacitance - Current_Left_Value);
        Delta_Right = (Captured_Right_Base_Capacitance - Current_Right_Value);
        
        Left_Position = (Delta_Left * 585)/(Delta_Left + Delta_Right);
        Right_Position = ((1 - Delta_Right) * 585)/(Delta_Left + Delta_Right);
        
        Position = (Left_Position + Right_Position)/2;
        
        sprintf(message, "CTMU Values: %i  %i", Current_Left_Value, Current_Right_Value);
        drawString(0,0,message);
        sprintf(message, "Position: %i", Position);
        drawString(0,10,message);
        ssd1306_update();
        
        brightness = ((float) (Position+500))/500;
        if ((Current_Left_Value < 500) | (Current_Right_Value < 500)) {
            myColorArray[1] = HSBtoRGB(120,1,brightness);
            myColorArray[2] = HSBtoRGB(120,1,brightness);
        }
        else {
            myColorArray[1] = HSBtoRGB(0,0,0);
            myColorArray[2] = HSBtoRGB(0,0,0);
        }
        
        ws2812b_setColor(myColorArray, 4);
        delay_ms(50);
        ssd1306_clear();
    }
}