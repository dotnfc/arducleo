// Basic example to use the DAC. Output a sawtooth signal.
// https://developer.mbed.org/teams/ST/code/Nucleo_sawtooth/
//
// Assembled by dotnfc as Arducleo Sample
// 2016/08/23


#include "mbed.h"
 
AnalogOut dac_output(PA_4);
 
int main()
{
    printf("Sawtooth example\n");
 
    while(1) {
        for (int idx16 = 0; idx16 < 0xFFFF; idx16 += 5) {
            dac_output.write_u16(idx16);
            wait_us(10);
        }
 
        dac_output.write_u16(0xFFFF);
        wait_us(100);
 
        for (int idx16 = 0xFFFF; idx16 > 0; idx16 -= 5) {
            dac_output.write_u16(idx16);
            wait_us(10);
        }
 
        dac_output.write_u16(0);
    }
}

 


