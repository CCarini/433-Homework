#ifndef SPI__H__
#define SPI__H__

void initSPI();
unsigned char spi_io(unsigned char o);
void test_waveform ();
void delay_ms(unsigned int t);
void output_waveform_point (char channel, unsigned short waveform_value);
void generate_triangle_wave (unsigned short arr[]);
void generate_sine_wave (unsigned short arr[]);

#endif // SPI__H__