// PWM output a 4Mhz Square Wave
// by dotnfc@163.com


#include "mbed.h"
#include "PwmOutEx.h"


PwmOutEx mypwm(PA_1, 4000000);
DigitalOut myled(LED1);

int main() 
{
    printf ("4Mhz frequency on PA1/A1 pin.\n");
    while(1) { 
        myled = !myled;
        wait(1);
    }
}
