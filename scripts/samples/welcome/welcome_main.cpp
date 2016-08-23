////////////////////////////////////////////////////////////////////////////////
// Unpacking Demo
// by dotnfc@163.com
// 2016/08/15 08:08:46

#include "mbed.h"

PwmOut buz(PB_1);
DigitalOut led(LED1);

/* color rgb leds */
PwmOut ledR(PC_6);
PwmOut ledG(PC_8);
PwmOut ledB(PC_9);

//Serial pc(SERIAL_TX, SERIAL_RX);

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


void analogWrite(mbed::PwmOut *prt, float spd) // write to PWM
{
    float a = spd / 256.0; // convert 0..256 to '%'
    //pc.printf ( "a - %d, (%f)\n", spd, a );
    *prt = a;
}

/*==============================================================================
 * \brief the color generating function 
 */
void color (unsigned char red, unsigned char green, unsigned char blue)	
{
     analogWrite(&ledR, 255-red);
     analogWrite(&ledB, 255-blue);
     analogWrite(&ledG, 255-green);
     //ledR = 255-red;
     //ledB = 255-blue;
     //ledG = 255-green;
}	 

void delay (int ms)
{
    wait_ms (ms);
}


/*==============================================================================
 * \brief main entry
 */
void loop();
void loop1();
int main ()
{
    ledR = ledG = ledB = 1;
    //pc.baud (115200);
    //led = 1;
    tone (120, 800);
    tone (200, 600);
    tone (300, 400);
    //
    //led = 0;
    
    while(1) {
        //loop1 ();
#if 0
        led = 1;        
        ledR = 1;   ledB = 0;   ledG = 0;   wait_ms (1000);
        
        led = 0;
        ledR = 0;   ledB = 1;   ledG = 0;   wait_ms (1000);
        
        led = 1;
        ledR = 0;   ledB = 0;   ledG = 1;   wait_ms (1000);
        
        led = 0;
        ledR = 0;   ledB = 0;   ledG = 0;   wait_ms (1000);
#endif // 0
    }
}

void loop1 () 	 // run over and over again 
{	
     //Basic colors:	 
     color(255,0,0);	//turn the RGB LED red  
     wait_ms (1000);	//delay for 1 second  
     color(0,255,0);	//turn the RGB LED green  
     wait_ms (1000);	//delay for 1 second  
     color(0,0,255);	//turn the RGB LED blue
     wait_ms (1000);	//delay for 1 second  

     //Example blended colors:	 
     //color(255,255,0);	//turn the RGB LED yellow  
     //wait_ms (1000);	//delay for 1 second  
     //color(255,255,255);	//turn the RGB LED white  
     //wait_ms (1000);	//delay for 1 second  
     //color(128,0,255);	//turn the RGB LED purple  
     //wait_ms (1000);	//delay for 1 second  
     //color(0,0,0);	//turn the RGB LED off  
     //wait_ms (1000);	//delay for 1 second  
}

#define FADESPEED 5     // make this higher to slow down
void loop() 
{
  int r, g, b;
 
  // fade from blue to violet
  for (r = 0; r < 256; r++) { 
    analogWrite(&ledR, r);
    delay(FADESPEED);
  } 
  // fade from violet to red
  for (b = 255; b > 0; b--) { 
    analogWrite(&ledB, b);
    delay(FADESPEED);
  } 
  // fade from red to yellow
  for (g = 0; g < 256; g++) { 
    analogWrite(&ledG, g);
    delay(FADESPEED);
  } 
  // fade from yellow to green
  for (r = 255; r > 0; r--) { 
    analogWrite(&ledR, r);
    delay(FADESPEED);
  } 
  // fade from green to teal
  for (b = 0; b < 256; b++) { 
    analogWrite(&ledB, b);
    delay(FADESPEED);
  } 
  // fade from teal to blue
  for (g = 255; g > 0; g--) { 
    analogWrite(&ledG, g);
    delay(FADESPEED);
  } 
}

