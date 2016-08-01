// Enter in sleep or deepsleep modes.
// https://developer.mbed.org/teams/ST/code/Nucleo_sleep/

#include "mbed.h"
 
InterruptIn event(USER_BUTTON);
DigitalOut myled(LED1);
Serial pc(SERIAL_TX, SERIAL_RX);
 
int go_to_sleep = 0;
 
void pressed()
{
    pc.printf("Button pressed\n");
    go_to_sleep = go_to_sleep + 1;
    if (go_to_sleep > 3) go_to_sleep = 0;
 
}
 
int main()
{
    int i = 0;
 
    pc.printf("\nPress Button to enter/exit sleep & deepsleep\n");
 
    event.fall(&pressed);
 
    while (1) {
 
        if ((go_to_sleep == 0) || (go_to_sleep == 2)) {
            pc.printf("%d: Running\n", i);
            myled = !myled;
            wait(1.0);
        }
 
        if (go_to_sleep == 1) {
            myled = 0;
            pc.printf("%d: Entering sleep (press user button to resume)\n", i);
            sleep();
        }
 
        if (go_to_sleep == 3) {
            myled = 0;
            pc.printf("%d: Entering deepsleep (press user button to resume)\n", i);
            deepsleep();
        }
 
        i++;
    }
}

 


