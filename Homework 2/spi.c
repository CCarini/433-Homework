#include <proc/p32mx170f256b.h>

#include "spi.h"
#include<xc.h> 
#include <math.h>

// initialize SPI1
void initSPI() {
    // Pin B14 has to be SCK1
    // Turn of analog pins
    ANSELA = 0;
    // Make an output pin for CS
    TRISAbits.TRISA0 = 0;
    LATAbits.LATA0 = 1;
    // Set SDO1
    RPA1Rbits.RPA1R = 0b0011;
    // Set SDI1
    SDI1Rbits.SDI1R = 0b0001;

    // setup SPI1
    SPI1CON = 0; // turn off the spi module and reset it
    SPI1BUF; // clear the rx buffer by reading from it
    SPI1BRG = 1; // 1000 for 24kHz, 1 for 12MHz; // baud rate to 10 MHz [SPI1BRG = (48000000/(2*desired))-1]
    SPI1STATbits.SPIROV = 0; // clear the overflow bit
    SPI1CONbits.CKE = 1; // data changes when clock goes from hi to lo (since CKP is 0)
    SPI1CONbits.MSTEN = 1; // master operation
    SPI1CONbits.ON = 1; // turn on spi 
}

unsigned char spi_io(unsigned char o) {
    SPI1BUF = o;
    while (!SPI1STATbits.SPIRBF) {
        ;
    }
    return SPI1BUF;
}

void test_waveform () {
    char msb = 0b01111000;
    char lsb = 0b00000000;
    LATAbits.LATA0 = 0; // Bring CS low
    spi_io(msb); // Send msb
    spi_io(lsb); // Send lsb
    LATAbits.LATA0 = 1; // Bring CS high
}

void output_waveform_point (char channel, unsigned short waveform_value) {
    char lsb = waveform_value;
    char msb = waveform_value >> 8;
    msb = msb | 0b11110000;
    if (channel == 'A') {
        msb = msb & 0b01111111;
    }
    LATAbits.LATA0 = 0; // Bring CS low
    spi_io(msb); // Send msb
    spi_io(lsb); // Send lsb
    LATAbits.LATA0 = 1; // Bring CS high   
}

void generate_triangle_wave (unsigned short arr[]) {
    int i = 0;
    double increment = 4096/500;
    double voltage_scaler = 0;
    for (i; i < 500; i++) {
        arr[i] = (unsigned int)voltage_scaler;
        voltage_scaler += increment;
    }
    for (i; i < 1000; i++) {
        voltage_scaler -= increment;
        arr[i] = (unsigned int)voltage_scaler;
    }
}

void generate_sine_wave (unsigned short arr[]) {
    int i = 0;
    unsigned short voltage_scaler = 0;
    double increment = 2*M_PI/500;
    for (i; i < 500; i++) {
        voltage_scaler = (unsigned int)(((sin(increment*i)*.5) + .5)*4095);
        arr[i] = voltage_scaler;
    }
}

void delay_ms (unsigned int t) {
     _CP0_SET_COUNT(0);
     while (_CP0_GET_COUNT() < (24000*t)) {
            
        }
}