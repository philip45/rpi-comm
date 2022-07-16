/**
 * @file funnet.h
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

#define HEADER_SIZE sizeof(uint8) * 3
#define PAYLOAD_SIZE 16
typedef uint8 payload_t[PAYLOAD_SIZE];

typedef struct {
    uint8 sender_id;
    uint8 listener_id;
    uint8 check_sum;
    payload_t data;
} FunFrame;

// typedef union {
//     FunFrame frame;
//     uint8 all_data[HEADER_SIZE + PAYLOAD_SIZE]
// } FunFrameUnion;

uint64 elapsed_time_us();
uint64 absolute(int64 x);
int32 timediff(uint64 new_stamp, uint64 old_stamp);

void ff_init(FunFrame *frame, uint8 sender_id, uint8 receiver_id, uint8 payload[]);
void ff_populate_paylolad(FunFrame *frame, payload_t payload);
uint8 ff_calc_check_sum(FunFrame *frame);
void ff_set_check_sum(FunFrame *frame);
int ff_send(FunFrame *frame);
