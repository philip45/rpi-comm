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

#define CHECKSUM_INITIAL 11

void ff_init(funframe_t *frame, uint8 sender_id, uint8 receiver_id, uint8 payload[]) {
    register uint8 checksum = CHECKSUM_INITIAL;

    checksum += (frame->sender_id = sender_id);
    checksum += (frame->listener_id = receiver_id);

    for (register int i = 0; i < FF_PAYLOAD_SIZE; i++) {
        frame->data[i] = payload[i];
        checksum += frame->data[i];
    }
    frame->check_sum = checksum;
}

uint8 ff_calc_check_sum(funframe_t *frame) {
    register uint8 checksum = CHECKSUM_INITIAL;

    checksum += frame->sender_id;
    checksum += frame->listener_id;

    for (register int i = 0; i < FF_PAYLOAD_SIZE; i++) {
        checksum += frame->data[i];
    }
    return checksum;
}

void ff_set_check_sum(funframe_t *frame) {
    register uint8 checksum = CHECKSUM_INITIAL;

    checksum += frame->sender_id;
    checksum += frame->listener_id;

    for (register int i = 0; i < FF_PAYLOAD_SIZE; i++) {
        checksum += frame->data[i];
    }
    frame->check_sum = checksum;
}

int ff_send(funframe_t *frame) {
    gpio_send_sync();

    funframe_full_t *frame_full = (funframe_full_t *)frame;

    for (register int i = 0; i < FF_FULL_SIZE; i++) {
        gpio_send_byte(frame_full->data[i]);
    }
    return 0;
}

int ff_receive(funframe_t *frame) {
    int rc = -1;
    if (rc = gpio_wait_sync()) {
        return rc;
    }

    funframe_full_t *frame_full = (funframe_full_t *)frame;

    for (int i = 0; i < FF_FULL_SIZE; i++) {
        frame_full->data[i] = gpio_receive_byte();
    }

    int actual_checksum = ff_calc_check_sum(frame);
    if (actual_checksum != frame->check_sum) {
        printf("Checksum mismatch! Expected %u but was %u\n", frame->check_sum, actual_checksum);
        return 9;
    }
    return 0;
}

void ff_print_payload(funframe_t *frame) {
    printf("[");
    for (int i = 0; i < FF_PAYLOAD_SIZE; i++) {
        printf(" %u,", frame->data[i]);
    }
    printf("]\n");
}

void ff_print(funframe_t *frame) {
    printf("(%u, %u, %u)", frame->sender_id, frame->listener_id, frame->check_sum);
    ff_print_payload(frame);
}
