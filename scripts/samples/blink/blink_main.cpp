// Basic example showing how to blink the LED present on the board.
// https://developer.mbed.org/teams/ST/code/Nucleo_blink_led/

#include "mbed.h"
 
DigitalOut myled(LED1);
 
int main() {  
    while(1) {
        myled = 1; // LED is ON
        wait(0.2); // 200 ms
        myled = 0; // LED is OFF
        wait(1.0); // 1 sec
    }
}

