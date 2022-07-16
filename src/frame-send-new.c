#include <stdio.h>

// #include <bcm2835.h>

#include "bcm-ext.h"
#include "funnet.h"

#define SEND_PIN RPI_GPIO_P1_11
#define RECEIVE_PIN RPI_GPIO_P1_15

const int T = 15000; // microseconds
const int T_SYNC_ON = 12 * T;
const int T_SYNC_OFF = 2 * T;

// typedef uint8 payload_t[16];

// typedef struct {
//    uint8 sender_id;
//    uint8 listener_id;
//    uint8 check_sum;
//    payload_t data;
// } FunFrame;

// void FunFrame_populate_paylolad(FunFrame *frame, payload_t payload) {
//    for (int i = 0; i < 16; i++) {
//       (*frame).data[i] = payload[i];
//    }
// }

// uint8 FunFrame_calc_check_sum(FunFrame *frame) {
//    uint8 check_sum;
//    for (int i = 0; i < 16; i++) {
//       check_sum += (*frame).data[i];
//    }
//    return check_sum;
// }

// void FunFrame_set_check_sum(FunFrame *frame) {
//    (*frame).check_sum = FunFrame_calc_check_sum(frame);
// }

// void send_bit(int lvl) {
//     if (lvl == 1) {
//         bcm2835_gpio_write(SEND_PIN, HIGH);
//         bcm2835_delayMicroseconds(T);
//         bcm2835_gpio_write(SEND_PIN, LOW);
//     } else {
//         bcm2835_gpio_write(SEND_PIN, LOW);
//         bcm2835_delayMicroseconds(T);
//     }
// }

// void send_byte(uint8 byte) {
//     for (uint8 mask = 0x80; mask != 0; mask >>= 1) {
//         if (byte & mask) {
//             send_bit(1);
//         } else {
//             send_bit(0);
//         }
//     }
// }

// void send_data(int pin, int bit_span, uint8 arr[]) {
//     for (int i = 0; i < 16; i++) {
//         for (uint8 mask = 0x80; mask != 0; mask >>= 1) {
//             if (arr[i] & mask) {
//                 send_one(pin, bit_span);
//             } else {
//                 send_zero(pin, bit_span);
//             }
//         }
//         send_bit(0);
//     }
// }

int main() {
    initialize(SEND_PIN, RECEIVE_PIN, T);

    FunFrame frame1;
    frame1.sender_id = 1;
    frame1.listener_id = 2;
    payload_t data = {5, 26, 75, 153, 52, 231, 12, 35, 222, 153, 56, 2, 162, 6, 9, 82};
    ff_populate_paylolad(&frame1, data);
    // data[3] = 99;
    // frame1.data[3] = 99;
    for (int i = 0; i < 16; i++) {
        printf(" %d", frame1.data[i]);
    }
    printf("\n");
    frame1.check_sum = ff_calc_check_sum(&frame1);

    // TODO:
    //  Implement sending as a FunFrame method and use it.

    // send_byte(frame1.sender_id);
    // send_byte(frame1.listener_id);
    // send_byte(frame1.check_sum);
    // send_data(frame1.data);

    return 0;
}
