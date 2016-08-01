// Output a PWM signal on any IO using Timeout.
// https://developer.mbed.org/teams/ST/code/Nucleo_pwm3/

#include "mbed.h"
 
Timeout timer;
DigitalOut my_led(LED1);
DigitalOut my_pwm(D10); // IO used by pwm_io function
 
int on_delay = 0;
int off_delay = 0;
 
void toggleOff(void);
 
void toggleOn(void) {
    my_pwm = 1;
    timer.attach_us(toggleOff, on_delay);
}
 
void toggleOff(void) {
    my_pwm = 0;
    timer.attach_us(toggleOn, off_delay);
}
 
// p_us = signal period in micro_seconds
// dc   = signal duty-cycle (0.0 to 1.0)
void pwm_io(int p_us, float dc) {
    timer.detach();
    if ((p_us == 0) || (dc == 0)) {
        my_pwm = 0;
        return;
    }
    if (dc >= 1) {
        my_pwm = 1;
        return;
    }
    on_delay = (int)(p_us * dc);
    off_delay = p_us - on_delay;
    toggleOn();
}
 
int main() {
    
    pwm_io(20000, 0.25); // 20ms - 25%
    
    while(1) {
        my_led = !my_led;
        wait(0.5);
    }
}
 

