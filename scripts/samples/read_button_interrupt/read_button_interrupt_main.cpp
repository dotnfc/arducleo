// Read the user button using external interrupt.
// https://developer.mbed.org/teams/ST/code/Nucleo_read_button_interrupt/

#include "mbed.h"
 
InterruptIn mybutton(USER_BUTTON);
DigitalOut myled(LED1);
 
float delay = 1.0; // 1 sec
 
void pressed()
{
    if (delay == 1.0)
        delay = 0.2; // 200 ms
    else
        delay = 1.0; // 1 sec
}
 
int main()
{
    mybutton.fall(&pressed);
    while (1) {
        myled = !myled;
        wait(delay);
    }
}
 

