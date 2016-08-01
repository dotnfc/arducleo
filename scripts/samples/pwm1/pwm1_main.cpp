// Output a pwm signal.
// https://developer.mbed.org/teams/ST/code/Nucleo_pwm/

#include "mbed.h"
 
PwmOut mypwm(PWM_OUT);
 
DigitalOut myled(LED1);
 
int main() {
    
    mypwm.period_ms(10);
    mypwm.pulsewidth_ms(1);
  
    printf("pwm set to %.2f %%\n", mypwm.read() * 100);
    
    while(1) {
        myled = !myled;
        wait(1);
    }
}


