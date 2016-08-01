// Toggle several IOs of a port at the same time.
// https://developer.mbed.org/teams/ST/code/Nucleo_toggle_ios/

#include "mbed.h"
 
#define IOS (0xA0) // PA_5 + PA_7
 
PortOut myIOs(PortA, IOS);
 
int main() {
    while(1) {   
        myIOs = myIOs ^ IOS; // Toggle IOs level
        wait(0.5); // 500 ms
    }
}


