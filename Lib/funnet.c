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

#include "funnet.h"
#include "bcm-ext.h"
#include <stdio.h>

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
        check_sum += frame->data[i];
    }
    return check_sum;
}

void ff_set_check_sum(FunFrame *frame) {
    register uint8 checksum = 0;
    for (register int i = 0; i < PAYLOAD_SIZE; i++) {
        checksum += frame->data[i];
    }
    frame->check_sum = checksum;
}

int ff_send(FunFrame *frame) {
    gpio_send_sync();
    gpio_send_byte(frame->sender_id);
    gpio_send_byte(frame->listener_id);
    gpio_send_byte(frame->check_sum);
    for (int i = 0; i < PAYLOAD_SIZE; i++) {
        gpio_send_byte(frame->data[i]);
    }
    return 0;
}

int ff_receive(FunFrame *frame) {
    int rc = -1;
    if (rc = gpio_wait_sync()) {
        return rc;
    }
    frame->sender_id = gpio_receive_byte();
    frame->listener_id = gpio_receive_byte();
    frame->check_sum = gpio_receive_byte();
    for (int i = 0; i < PAYLOAD_SIZE; i++) {
        frame->data[i] = gpio_receive_byte();
    }
    int actual_checksum = ff_calc_check_sum(frame);
    if (actual_checksum != frame->check_sum) {
        printf("Checksum mismatch! Expected %u but was %u", frame->check_sum, actual_checksum);
        return 9;
    }
    return 0;
}

void ff_print_payload(FunFrame *frame) {
    printf("[");
    for (int i = 0; i < PAYLOAD_SIZE; i++) {
        printf(" %u,", frame->data[i]);
    }
    printf("]\n");
}
