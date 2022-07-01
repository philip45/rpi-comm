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

int initialize(uint8 pin, uint8 mode) {

    printf("Initializing (pin=%u, mode=%u)... ", pin, mode);

    if (!bcm2835_init()) {
        printf("FAILED!\n");
        return 1;
    }

    switch (mode) {
    case BCM2835_GPIO_FSEL_INPT:
        bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_INPT);
        bcm2835_gpio_set_pad(pin, BCM2835_GPIO_PUD_DOWN);
        break;

    case BCM2835_GPIO_FSEL_OUTP:
        bcm2835_gpio_fsel(pin, BCM2835_GPIO_FSEL_OUTP);
        bcm2835_gpio_write(pin, LOW);
        break;

    default:
        printf("UNREACHABLE: unknown mode %d\n", mode);
        return 9;
    }

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
