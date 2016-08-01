// Read the user button state on the Nucleo board.
// https://developer.mbed.org/teams/ST/code/Nucleo_read_button/

#include "mbed.h"
 
DigitalIn mybutton(USER_BUTTON);
DigitalOut myled(LED1);
 
int main() {
  while(1) {
    if (mybutton == 0) { // Button is pressed
      myled = !myled; // Toggle the LED state
      wait(0.2); // 200 ms
    }
  }
}

