// Send data with SPI.
// https://developer.mbed.org/teams/ST/code/Nucleo_spi_master/

#include "mbed.h"
 
SPI device(SPI_MOSI, SPI_MISO, SPI_SCK);
 
int main() {
    int i = 0;
    while(1) {
        device.write(0x55);
        device.write(i++);
        device.write(0xE0);
        wait_us(50);
    }
}


