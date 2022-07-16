/**
 * @file funnet.c
 * @author Philip Damyanov
 * @author Yassen Damyanov
 * @brief
 * @version 0.0.1
 * @date 2022-06-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <stdio.h>
#include <sys/time.h>

#include "bcm-ext.h"
#include "funnet.h"

#define US_IN_1SEC 1000000

uint64 absolute(int64 x) {
    if (x < 0) return -x;
    return x;
}

uint64 elapsed_time_us() {
    struct timeval tval;

    gettimeofday(&tval, NULL);
    uint64 useconds = tval.tv_usec;
    return useconds;
}

int32 timediff(uint64 new_stamp, uint64 old_stamp) {
    if (new_stamp >= old_stamp) {
        return new_stamp - old_stamp;
    }
    return US_IN_1SEC - old_stamp + new_stamp;
}

void ff_init(FunFrame *frame, uint8 sender_id, uint8 receiver_id, uint8 payload[]) {
    frame->sender_id = sender_id;
    frame->listener_id = receiver_id;
    register uint8 checksum = 0;
    for (register int i = 0; i < PAYLOAD_SIZE; i++) {
        frame->data[i] = payload[i];
        checksum += frame->data[i];
    }
    frame->check_sum = checksum;
}

uint8 ff_calc_check_sum(FunFrame *frame) {
    uint8 check_sum;
    for (int i = 0; i < 16; i++) {
        check_sum += (*frame).data[i];
    }
    return check_sum;
}

void ff_set_check_sum(FunFrame *frame) {
    (*frame).check_sum = ff_calc_check_sum(frame);
}

int ff_send(FunFrame *frame) {
    gpio_send_sync();
    gpio_send_byte(frame->sender_id);
    gpio_send_byte(frame->listener_id);
    gpio_send_byte(frame->check_sum);
    for (int i = 0; i < PAYLOAD_SIZE; i++) {
        gpio_send_byte(frame->data[i]);
    }
}
