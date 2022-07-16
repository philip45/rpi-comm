
#include <stdio.h>

#include "../Lib/bcm-ext.h"
#include "../Lib/funnet.h"

#define SEND_PIN 11
#define RECEIVE_PIN 15

int main() {
    initialize(SEND_PIN, RECEIVE_PIN);

    FunFrame frame1;
    payload_t data = {5, 26, 75, 153, 52, 231, 12, 35, 222, 153, 56, 2, 162, 6, 9, 82};
    ff_init(&frame1, 1, 2, data);

    printf("\n");
    // frame1.check_sum = ff_calc_check_sum(&frame1);

    return 0;
}
