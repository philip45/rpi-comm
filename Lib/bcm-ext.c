/**
 * @file libbcm-ext.c
 * @author Philip Damyanov
 * @author Yassen Damyanov
 * @brief
 * @version 0.0.1
 * @date 2022-06-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "bcm-ext.h"
#include <bcm2835.h>
#include <stdio.h>

gpio_params_t gpio_params = {255, 255};

int initialize(uint8 send_pin, uint8 receive_pin) {

    gpio_params.send_pin = send_pin;
    gpio_params.receive_pin = receive_pin;

    // gpio_params.bit_span = bit_span;
    // gpio_params.sync_span = sync_span;

    printf("Initializing (send_pin=%u, receive_pin=%u, bit_span=%d)... ", send_pin, receive_pin);

    if (!bcm2835_init()) {
        printf("FAILED!\n");
        return 1;
    }

    // Initilize receive_pin
    bcm2835_gpio_fsel(receive_pin, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pad(receive_pin, BCM2835_GPIO_PUD_DOWN);

    // Initilize send_pin
    bcm2835_gpio_fsel(send_pin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(send_pin, LOW);

    printf("OK.\n");
    return 0;
}

int cleanup() {
    if (!bcm2835_close()) {
        printf("Clenup FALIED!\n");
        return 1;
    }
    printf("Cleanup OK\n");
    return 0;
}

// void send_bit(int pin, int lvl, int bit_span) {
//    if (lvl == 1) {
//       bcm2835_gpio_write(pin, HIGH);
//       bcm2835_delayMicroseconds(bit_span);
//       bcm2835_gpio_write(pin, LOW);
//    } else {
//       bcm2835_gpio_write(pin, LOW);
//       bcm2835_delayMicroseconds(bit_span);
//    }
// }

void send_one() {
    bcm2835_gpio_write(gpio_params.send_pin, HIGH);
    bcm2835_delayMicroseconds(T);
    // bcm2835_gpio_write(gpio_params.send_pin, LOW);
}

void send_zero() {
    bcm2835_gpio_write(gpio_params.send_pin, LOW);
    bcm2835_delayMicroseconds(T);
    // bcm2835_gpio_write(gpio_params.send_pin, LOW);
}

void send_byte(uint8 byte) {
    for (uint8 mask = 0x80; mask != 0; mask >>= 1) {
        if (byte & mask) {
            send_one();
        } else {
            send_zero();
        }
    }
}

void send_sync() {
    bcm2835_gpio_write(gpio_params.send_pin, HIGH);
    bcm2835_delayMicroseconds(T_SYNC_ON);
    bcm2835_gpio_write(gpio_params.send_pin, LOW);
    bcm2835_delayMicroseconds(T_SYNC_OFF);
}
