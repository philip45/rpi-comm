#include <bcm2835.h>
#include <stdio.h>

#define PIN RPI_GPIO_P1_11
#define uint8 __uint8_t

const int T = 1000;  // microseconds
const int T_ZERO_ON = T/3;
const int T_ZERO_OFF = 2 * T/3;
const int T_ONE_ON = 2 * T/3;
const int T_ONE_OFF = T/3;
const int T_SYNC_ON = 3 * T/2;
const int T_SYNC_OFF = T/2;


void send_pulse(int time_on, int time_off) {
    bcm2835_gpio_write(PIN, HIGH);
    bcm2835_delayMicroseconds(time_on);
    bcm2835_gpio_write(PIN, LOW);
    bcm2835_delayMicroseconds(time_off);
}

void send_sync() {
    send_pulse(T_SYNC_ON, T_SYNC_OFF);
}

void send_zero() {
    send_pulse(T_ZERO_ON, T_ZERO_OFF);
}

void send_one() {
    send_pulse(T_ONE_ON, T_ONE_OFF);
}


void send_byte(uint8 byte) {
    send_sync();

    uint8 filtered;

    for (uint8 mask = 0x80; mask != 0; mask >>= 1) {
        filtered = byte & mask;

        if ((filtered & mask) == 0) {
            send_zero();
        } else {
            send_one();
        }
    }
}


void initialize() {
    bcm2835_init();
    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_OUTP);
    printf("bcm2835 initilized.\n");
}


int main(/*int argc, char *argv[]*/) {
    // if (argc != 2) {
    //     printf("Wrong number of arguments(%u)", argc);
    //     printf("Usage:  send {MESSAGE-TO-BE-SENT}");
    //     return 1;
    // }

    initialize();

    printf("Sending first group...\n");

    send_byte(5);
    send_byte(9);
    send_byte(254);
    send_byte(5);
    send_byte(9);
    send_byte(254);
    send_byte(5);
    send_byte(9);
    send_byte(254);
    send_byte(5);
    send_byte(9);
    send_byte(254);
    send_byte(5);
    send_byte(9);
    send_byte(254);
    send_byte(5);
    send_byte(9);
    send_byte(254);

    // printf("Sending second group...\n");

    // send_byte(5);
    // send_byte(9);
    // send_byte(254);

    printf("DONE.\n");
    return 0;
}
