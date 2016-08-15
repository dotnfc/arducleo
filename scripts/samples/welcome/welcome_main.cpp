////////////////////////////////////////////////////////////////////////////////
// Unpacking Demo
// by dotnfc@163.com
// 2016/08/15 08:08:46

#include "mbed.h"

PwmOut buz(PB_1);
DigitalOut led(LED1);

/* color rgb leds */
DigitalOut ledR(PC_6);
DigitalOut ledG(PC_8);
DigitalOut ledB(PC_9);


/*==============================================================================
 * \brief arduino like 'tone ()'
 */
void tone(int frequency, float duration) 
{
    float period;

    // calculate the period of the note in turn play
    period = 1000000 / frequency;
    // Activate the LED of the core to indicate that we are playing a note
    led = 1;
    // set the period calculated at the PWM output
    buz.period_us (period);
    
    // establish a duty cycle of 50%
    buz.write (0.50f);
    // hope the time set by the tone i
    wait_ms (duration);
    // disabled the core led to indicate the end of the note
    led = 0;
    // ended the note
    buz = 0;
}


/*==============================================================================
 * \brief main entry
 */
int main ()
{
    tone (100000, 100);
    tone (100000, 100);
    
    while(1) {
    
    }
}