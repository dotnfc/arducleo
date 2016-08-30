// Read external LM75 temperature sensor using I2C master.
// https://developer.mbed.org/teams/ST/code/Nucleo_i2c_master/
//
// Assembled by dotnfc as Arducleo Sample
// 2016/08/23

// notes for i2c
// 1. Aardvark 的地址是纯地址，不包括读写位。比如 mbed 中 addr=0xa0, aardvard 中 addr=0x50
// 2. i2c 的 slave 的读取操作必须和 master 的写入数据长度一致，否则会导致通讯失败
// 3. 默认 baud=100000 Hz, max_baud=400000Hz

#include "mbed.h"
 
#define LM75_REG_TEMP (0xa5) // Temperature Register
#define LM75_REG_CONF (0x5a) // Configuration Register
#define LM75_ADDR     (0x48) // LM75 address
 

 
DigitalOut myled(LED1);
 
Serial pc(SERIAL_TX, SERIAL_RX);
 
#if 0 
int main_master()
{
    I2C i2c(I2C_SDA, I2C_SCL);
    int count = 0;
    char data_write[] = "hello slave.";
    char data_read[24];
    
    while (1) {

        int status = i2c.write(LM75_ADDR, data_write, sizeof(data_write), 0);
        if (status != 0) { // Error
            printf ( "slave not ack.\n" );
            while (1) {
                myled = !myled;
                wait(0.2);
            }
        }
        
        count = 5;
        do {
            status = i2c.read(LM75_ADDR, data_read, 2, 0);
            if (status == 0) {
                printf ( "%02X %02X\n", data_read[0], data_read[1] );
                break;
            }
            else {
                count --;
                wait (0.5);
            }
        } while (count > 0);
                
        myled = !myled;
        wait(0.6);
    } 
}
#endif 


int main ()
{
    I2CSlave slave(I2C_SDA, I2C_SCL);
    
    char buf[10];
    char msg[] = "Slave!";

    slave.address(LM75_ADDR);
    while (1) {
        int i = slave.receive();
        switch (i) {
            case I2CSlave::ReadAddressed:
                slave.write(msg, strlen(msg) + 1); // Includes null char
                //printf ( "read me %s.\n", msg );
                break;
            case I2CSlave::WriteGeneral:
                slave.read(buf, 3);
                //printf("Read G: %s\n", buf);
                break;
            case I2CSlave::WriteAddressed:
                slave.read(buf, 3);
                //printf("Read A: %s\n", buf);
                break;
            default:
                //printf ( "unknown ins: %d\n", i );
                //slave.stop ();
                break;
        }
        for(int i = 0; i < 10; i++) buf[i] = 0;    // Clear buffer
        
        myled = !myled;
        //wait (2);
    }
}
