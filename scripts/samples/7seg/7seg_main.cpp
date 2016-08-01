// Basic example to display a counter on the 7 segment Gravitech Arduino Nano
// https://developer.mbed.org/teams/ST/code/Nucleo32_gravitech_7seg/

#include "mbed.h"
 
#define SLAVE_ADDRESS (0x70) // SAA1064 component address
 
I2C i2c(PB_7, PB_6); // SDA, SCL
 
DigitalOut led1(LED1);
static int Digit4_Seg7_Display(uint32_t Value);
 
int main()
{
    int counter = 0;
    char control[2] = {0, 0x47};
 
    i2c.frequency(100000);
 
    /* init of the 4 digits 7 Segment Display shield for Arduino Nano - Gravitech. */
    i2c.write(SLAVE_ADDRESS, control, sizeof(control), 0);
 
    /* Infinite loop */
    while(1) {
        /* Stop the demonstration on 3232! */
        if(counter == 3232) {
            while(1) {
                /* Display the counter on the screen */
                if(Digit4_Seg7_Display(3232) != 0) {
                    /* Communication Error */
                    while (1) {}
                }
 
                /* Add some delay */
                wait_ms(200);
 
                /* Display the counter on the screen */
                if(Digit4_Seg7_Display(10000) != 0) {
                    /* Communication Error */
                    while (1) {}
                }
 
                /* Add some delay */
                wait_ms(100);
            }
        }
 
        /* Display the counter on the screen */
        if(Digit4_Seg7_Display(counter++ % 10000) != 0) {
            /* Communication Error */
            while (1) {}
        }
 
        /* Add some delay */
        wait_ms(100);
        led1 = !led1;
    }
}
 
 
/**
  * @brief  Display of the 4 digits 7 Segment Display shield for Arduino Nano - Gravitech.
            Display the value if value belong to [0-9999]
  * @param  Value A number between 0 and 9999 will be displayed on the screen.
  *         DIGIT4_SEG7_RESET will reset the screen (any value above 9999 will reset the screen also)
  * @retval HAL_StatusTypeDef
  */
int Digit4_Seg7_Display(uint32_t Value)
{
    const char lookup[10] = {0x3F,0x06,0x5B,0x4F,0x66,
                             0x6D,0x7D,0x07,0x7F,0x6F
                            };
 
    int thousands, hundreds, tens, base;
    int status = 1;
    char d1d2d3d4[5];
    d1d2d3d4[0] = 1; // register address in the device
    if (Value < 10000) {
        thousands = Value / 1000;
        hundreds = (Value - (thousands * 1000)) / 100;
        tens = (Value - ((thousands * 1000) + (hundreds * 100))) / 10;
        base = Value - ((thousands * 1000) + (hundreds * 100) + (tens * 10));
 
        d1d2d3d4[4] = lookup[thousands];
        d1d2d3d4[3] = lookup[hundreds];
        d1d2d3d4[2] = lookup[tens];
        d1d2d3d4[1] = lookup[base];
    } else {
        d1d2d3d4[4] = 0;
        d1d2d3d4[3] = 0;
        d1d2d3d4[2] = 0;
        d1d2d3d4[1] = 0;
    }
 
    /* Send the four digits to the SAA1064 component */
    status = i2c.write(SLAVE_ADDRESS, d1d2d3d4, sizeof(d1d2d3d4), 0);
 
    return status;
}
