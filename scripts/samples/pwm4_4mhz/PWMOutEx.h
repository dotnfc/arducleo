#ifndef MBED_PWMOUT_EX_H
#define MBED_PWMOUT_EX_H

#include "platform.h"


#if DEVICE_PWMOUT
#include "pwmout_api.h"


/*
#include "mbed.h"
#include "PwmOutEx.h"

// generate a 4Mhz clock on STM32F103RB
PwmOutEx mypwm(PA_1, 4000000);
DigitalOut myled(LED1);

int main() 
{
    while(1) { 
        myled = !myled;
        wait(1);
    }
}
*/


#ifdef __cplusplus
extern "C" {
#endif
    
void pwmout_init_ex(pwmout_t* obj, PinName pin, uint32_t freq);

#ifdef __cplusplus
}
#endif

namespace mbed {

class PwmOutEx {

public:

    /** Create a PwmOut connected to the specified pin
     *
     *  @param pin PwmOut pin to connect to
     */
    PwmOutEx(PinName pin, uint32_t freq) {
        pwmout_init_ex(&_pwm, pin, freq);
    }
    
protected:
    pwmout_t _pwm;

};

} // namespace mbed

#endif // DEVICE_PWMOUT

#endif // MBED_PWMOUT_EX_H
