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

#define T 5000 // microseconds
#define T_SYNC_ON 16 * T
#define T_SYNC_OFF 4 * T

uint64 elapsed_time_us();
uint64 absolute(int64 x);
int32 timediff(uint64 new_stamp, uint64 old_stamp);

int gpio_init(uint8 send_pin, uint8 receive_pin);
int gpio_cleanup();
void gpio_send_one();
void gpio_send_zero();
void gpio_send_byte(uint8 byte);
void gpio_send_sync();
void sleep(int times);
int gpio_wait_sync();
uint8 gpio_receive_byte();
