// Read external LM75 temperature sensor using I2C master.
// https://developer.mbed.org/teams/ST/code/Nucleo_i2c_master/
//
// Assembled by dotnfc as Arducleo Sample
// 2016/08/23


#include "mbed.h"
 
#define LM75_REG_TEMP (0x00) // Temperature Register
#define LM75_REG_CONF (0x01) // Configuration Register
#define LM75_ADDR     (0xA0) // LM75 address
 
I2C i2c(I2C_SDA, I2C_SCL);
 
DigitalOut myled(LED1);
 
Serial pc(SERIAL_TX, SERIAL_RX);
 
volatile char TempCelsiusDisplay[] = "+abc.d C";
 
int main()
{
 
    char data_write[2];
    char data_read[2];
 
    /* Configure the Temperature sensor device STLM75:
    - Thermostat mode Interrupt
    - Fault tolerance: 0
    */
    data_write[0] = LM75_REG_CONF;
    data_write[1] = 0x02;
    int status = i2c.write(LM75_ADDR, data_write, 2, 0);
    if (status != 0) { // Error
        while (1) {
            myled = !myled;
            wait(0.2);
        }
    }
 
    while (1) {
        // Read temperature register
        data_write[0] = LM75_REG_TEMP;
        i2c.write(LM75_ADDR, data_write, 1, 1); // no stop
        i2c.read(LM75_ADDR, data_read, 2, 0);
 
        // Calculate temperature value in Celcius
        int tempval = (int)((int)data_read[0] << 8) | data_read[1];
        tempval >>= 7;
        if (tempval <= 256) {
            TempCelsiusDisplay[0] = '+';
        } else {
            TempCelsiusDisplay[0] = '-';
            tempval = 512 - tempval;
        }
 
        // Decimal part (0.5¡ãC precision)
        if (tempval & 0x01) {
            TempCelsiusDisplay[5] = 0x05 + 0x30;
        } else {
            TempCelsiusDisplay[5] = 0x00 + 0x30;
        }
 
        // Integer part
        tempval >>= 1;
        TempCelsiusDisplay[1] = (tempval / 100) + 0x30;
        TempCelsiusDisplay[2] = ((tempval % 100) / 10) + 0x30;
        TempCelsiusDisplay[3] = ((tempval % 100) % 10) + 0x30;
 
        // Display result
        pc.printf("temp = %s\n", TempCelsiusDisplay);
        myled = !myled;
        wait(1.0);
    }
 
}
