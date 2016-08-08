////////////////////////////////////////////////////////////////////////////////
// Sing mario world using PWM signal
// by dotnfc@163.com
// 2016/08/08 11:44:37

#include "mbed.h"
#include "mario.h"


DigitalOut my_led(LED1);
PwmOut my_pwm(PB_1); // IO used by pwm_io function

void buzz(long frequency, long length);
void sing(int s);


int main ()
{
  while(1) {
    //sing the tunes
    sing(1);
    sing(1);
    sing(2);
  }
}


void sing(int theme) 
{
  // iterate over the notes of the melody:
  if (theme == 2) {
    printf (" 'Underworld Theme'");
    int size = sizeof(underworld_melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {
 
      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / underworld_tempo[thisNote];
 
      buzz(underworld_melody[thisNote], noteDuration);
 
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      wait_ms (pauseBetweenNotes);
 
      // stop the tone playing:
      buzz(0, noteDuration); 
    } 
  } 
  else {
 
    printf (" 'Mario Theme'");
    int size = sizeof(melody) / sizeof(int);
    for (int thisNote = 0; thisNote < size; thisNote++) {
 
      // to calculate the note duration, take one second
      // divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000 / tempo[thisNote];
 
      buzz(melody[thisNote], noteDuration);
 
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.30;
      wait_ms (pauseBetweenNotes);
 
      // stop the tone playing:
      buzz(0, noteDuration); 
    }
  }
}
 
/*==============================================================================
 * \brief arduino like 'tone ()'
 */
void buzz(long frequency, long length) 
{
    if (frequency == 0) {
        my_pwm = 0;
        return;
    }
    
    float period;
    // Activate the LED of the core to indicate that we are playing a note
    my_led = 1;

    // calculate the period of the note in turn play
    period = 1000000 / frequency;
    // set the period calculated at the PWM output
    my_pwm.period_us (period);
    // establish a duty cycle of 50%
    my_pwm.write (0.50f);
    // hope the time set by the tone i
    wait_ms (length);
    // ended the note
    my_pwm = 0;
    
    // disabled the core led to indicate the end of the note
    my_led = 0;
}
 

