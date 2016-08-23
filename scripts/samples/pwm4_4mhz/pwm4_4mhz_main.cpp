// PWM output a 4Mhz Square Wave
// by dotnfc@163.com
//
// Assembled by dotnfc as Arducleo Sample
// 2016/08/23


#include "mbed.h"
#include "PwmOutEx.h"


PwmOutEx mypwm(PWM_OUT, 4000000);
DigitalOut myled(LED1);

int main() 
{
    printf ("4Mhz frequency on PA1/A1 pin.\n");
    while(1) { 
        myled = !myled;
        wait(1);
    }
}


