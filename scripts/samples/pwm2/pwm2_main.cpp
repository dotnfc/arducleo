// Output pwm signal with duty-cycle changed when pressing a button.
// https://developer.mbed.org/teams/ST/code/Nucleo_pwm2/
//
// Assembled by dotnfc as Arducleo Sample
// 2016/08/23


#include "mbed.h"
 
DigitalOut  my_led(LED1);
InterruptIn my_button(USER_BUTTON);
PwmOut      my_pwm(PB_3);
 
void pressed() {
    if (my_pwm.read() == 0.25) {
        my_pwm.write(0.75);
    }
    else {
        my_pwm.write(0.25);
    }
}
 
int main()
{
    // Set PWM
    my_pwm.period_ms(10);
    my_pwm.write(0.5);
    
    // Set button
    my_button.fall(&pressed);
    
    while (1) {
        my_led = !my_led;
        wait(0.5); // 500 ms
    }
}

