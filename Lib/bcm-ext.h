/**
 * @file bcm-ext.h
 * @author Philip Damyanov
 * @author Yassen Damyanov
 * @brief
 * @version 0.0.1
 * @date 2022-06-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "_types.h"

typedef struct {
    uint8 send_pin;
    uint8 receive_pin;
    // uint16 bit_span;
    // uint16 sync_span;
} gpio_params_t;

#define T 15000 // microseconds
#define T_SYNC_ON 12 * T
#define T_SYNC_OFF 2 * T

int gpio_init(uint8 send_pin, uint8 receive_pin);
int gpio_cleanup();
void gpio_send_one();
void gpio_send_zero();
void gpio_send_byte(uint8 byte);
void gpio_send_sync();
