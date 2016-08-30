// Hello World to sweep a servo through its full range
// https://developer.mbed.org/cookbook/Servo
//
// -------------------------- Wiring -------------------------- 
//  Red    - 5V
//  Brown  - Gnd
//  Yellow - D3 (PWM_OUT)
//
// Assembled by dotnfc as Arducleo Sample
// 2016/08/23


 
#include "mbed.h"
#include "Servo.h"
 
Servo myservo(PWM_OUT);
DigitalOut myled(LED1);
 
int main() {
    
    while (1) {
        for(float p=0; p<1.0; p += 0.05) {
            myservo = p;
            wait(0.2);
            myled = !myled;
        }
    }
}

