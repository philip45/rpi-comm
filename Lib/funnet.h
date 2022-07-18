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
#define FF_PAYLOAD_SIZE 16
typedef uint8 payload_t[FF_PAYLOAD_SIZE];

typedef struct {
    uint8 sender_id;
    uint8 listener_id;
    uint8 check_sum;
    payload_t data;
} funframe_t;

#define FF_FULL_SIZE sizeof(funframe_t)

typedef struct {
    uint8 data[FF_FULL_SIZE];
} funframe_full_t;

// typedef union {
//     funframe_t frame;
//     uint8 all_data[HEADER_SIZE + FF_PAYLOAD_SIZE]
// } FunFrameUnion;

void ff_init(funframe_t *frame, uint8 sender_id, uint8 receiver_id, uint8 payload[]);
void ff_populate_paylolad(funframe_t *frame, payload_t payload);
uint8 ff_calc_check_sum(funframe_t *frame);
void ff_set_check_sum(funframe_t *frame);
int ff_send(funframe_t *frame);
int ff_receive(funframe_t *frame);
void ff_print_payload(funframe_t *frame);
void ff_print(funframe_t *frame);
