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
#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>

#define US_IN_1SEC 1000000

const int32 T_SCAN = T / 50;
const int32 MARGIN = 3 * T_SCAN;
const int32 SMALL_DELAY = T / 200;
const int32 TUNED_T = T - T / 401;

typedef struct {
    uint8 send_pin;
    uint8 receive_pin;
} gpio_params_t;

gpio_params_t gpio_params = {255, 255};

// General purpose utility functions --------------

uint64 absolute(int64 x) {
    if (x < 0) return -x;
    return x;
}

uint64 elapsed_time_us() {
    struct timeval tval;

    gettimeofday(&tval, NULL);
    return tval.tv_usec;
    // return useconds;
}

int32 timediff(uint64 new_stamp, uint64 old_stamp) {
    if (new_stamp >= old_stamp) {
        return new_stamp - old_stamp;
    }
    return US_IN_1SEC - old_stamp + new_stamp;
}

// GPIO related helper functions --------------

int gpio_init(uint8 send_pin, uint8 receive_pin) {

    gpio_params.send_pin = send_pin;
    gpio_params.receive_pin = receive_pin;

    printf("gpio initialize (send_pin=%u, receive_pin=%u)... ", send_pin, receive_pin);

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

int gpio_cleanup() {
    if (!bcm2835_close()) {
        printf("Clenup FALIED!\n");
        return 1;
    }
    printf("gpio cleanup OK\n");
    return 0;
}

inline void gpio_send_one() {
    bcm2835_gpio_write(gpio_params.send_pin, HIGH);
    bcm2835_delayMicroseconds(T);
}

inline void gpio_send_zero() {
    bcm2835_gpio_write(gpio_params.send_pin, LOW);
    bcm2835_delayMicroseconds(T);
}

inline void gpio_send_byte(uint8 byte) {
    for (uint8 mask = 0x80; mask != 0; mask >>= 1) {
        if (byte & mask) {
            gpio_send_one();
        } else {
            gpio_send_zero();
        }
    }
    gpio_send_zero();
}

inline void gpio_send_sync() {
    bcm2835_gpio_write(gpio_params.send_pin, HIGH);
    bcm2835_delayMicroseconds(T_SYNC_ON);
    bcm2835_gpio_write(gpio_params.send_pin, LOW);
    bcm2835_delayMicroseconds(T_SYNC_OFF);
}

/**
 * Block reading the input pin until a sync pulse is detected and return zero
 * immediatelty. Return 1 on timeout.
 */
inline int gpio_wait_sync() {
    register uint8 level = 255;
    register uint8 last_level = 255;
    register int64 rising_stamp = -1;
    register int64 falling_stamp = -1;
    register int32 pulse_span = -1;
    register bool rising_detected = false;

    // printf("Waiting for SYNC...\n");

    // If SMALL_DELAY is OFF, i<350000000 =~ 30 seconds.
    // If SMALL_DELAY is ON, i<8000 =~ 14 seconds.
    for (register int i = 0; i < 54000; i++) {
        level = bcm2835_gpio_lev(gpio_params.receive_pin);
        // printf(" Level -> %d\n", level);

        if (last_level == LOW && level == HIGH) { // rising
            rising_stamp = elapsed_time_us();
            rising_detected = true;
            // printf("Rising detected\n");

        } else if (last_level == HIGH && level == LOW) { // falling
            // printf("Falling detected\n");
            if (!rising_detected) {
                printf("Falling detected - but NO rising yet! Continue.\n");
                last_level = level;
                continue;
            }

            falling_stamp = elapsed_time_us();
            pulse_span = timediff(falling_stamp, rising_stamp);
            if (absolute(pulse_span - T_SYNC_ON) < MARGIN) {
                // printf("SYNC\n");
                bcm2835_delayMicroseconds(T_SYNC_OFF + T / 8);
                return 0;
            }
        }

        last_level = level;
        bcm2835_delayMicroseconds(SMALL_DELAY);
    }
    printf("wait_for_sync timed out!\n");
    return 1;
}

inline uint8 gpio_receive_byte() {
    register uint8 pin = gpio_params.receive_pin;
    register uint8 result = 0;
    register int32 tuned_delay = TUNED_T;

    for (register uint8 i = 0; i < 7; i++) {
        result |= bcm2835_gpio_lev(pin);
        result <<= 1;
        bcm2835_delayMicroseconds(tuned_delay);
    }
    result |= bcm2835_gpio_lev(pin);
    bcm2835_delayMicroseconds(tuned_delay); // wait for the last bit span
    bcm2835_delayMicroseconds(tuned_delay); // skip the stop bit
    return result;
}
