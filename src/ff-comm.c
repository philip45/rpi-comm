#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../Lib/bcm-ext.h"
#include "../Lib/funnet.h"

#define SEND_PIN 11
#define RECEIVE_PIN 15

int main(int argc, char **argv) {
    gpio_init(SEND_PIN, RECEIVE_PIN);

    FunFrame frame1;
    payload_t data = {5, 26, 75, 153, 52, 231, 12, 35, 222, 153, 56, 2, 162, 6, 9, 82};
    ff_init(&frame1, 41, 42, data);

    gpio_cleanup();
    return 0;
}
