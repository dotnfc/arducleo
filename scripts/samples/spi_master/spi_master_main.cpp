// Send data with SPI.
// https://developer.mbed.org/teams/ST/code/Nucleo_spi_master/
//
// Assembled by dotnfc as Arducleo Sample
// 2016/08/23


#include "mbed.h"
// notes for spi
// 1. default bits=8, mode=0(POL=PHA=0), baud=1Mhz




int main_master() 
//int main() 
{
    int i = 0;
    uint8_t buf[5];
    SPI device(SPI_MOSI, SPI_MISO, SPI_SCK);
    DigitalOut cs(SPI_CS); // ssel

    while(1) {
        cs = 0;
        buf[0] = device.write(0x55);
        buf[1] = device.write(i++);
        buf[2] = device.write(0);
        
        //device.transfer (cmd, sizeof(cmd), buf, sizeof(buf));
        cs = 1;
        
        printf ( "%02X %02X %02X \n", buf[0], buf[1], buf[2] );
        wait(1);
    }
    
    return 1;
}


int main()
{
    // note: SPI_CS = PB6 is not SPI1_SS pin.
    //       SPI1_SS pin: PA4, PA15.
    SPISlave device(SPI_MOSI, SPI_MISO, SPI_SCK, PA_4);
    //DigitalOut cs(SPI_CS); // ssel
    //device.reply(0x00);              // Prime SPI with first reply
    while(1) {
        if(device.receive()) {
            int v = device.read();   // Read byte from master
            //v = (v + 1) % 0x100;     // Add one to it, modulo 256
            device.reply(v + 1);         // Make this the next reply
        }
    }
    
    return 1;
}

