#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../Lib/bcm-ext.h"
#include "../Lib/funnet.h"

#define SEND_PIN 17
#define RECEIVE_PIN 22
#define COUNT_FRAMES 10

const char *OPT_VALUES_SEND[] = {"send", "s", "@END"};
const char *OPT_VALUES_RECEIVE[] = {"receive", "r", "listen", "l", "@END"};
const char *OPT_VALUES_MODE[] = {"--mode", "-m", "@END"};

bool mostly_receive = false;

bool is_one_of(char *token, const char *options[]) {
    for (int i = 0;; i++) {
        if (0 == strcmp(options[i], "@END")) {
            return false;
        }
        if (0 == strcmp(token, options[i])) {
            return true;
        }
    }
    printf("UNREACHABLE! Did not get the @END element");
    return false;
}

void print_usage(char *prog_name) {
    printf("USAGE:  %s --mode|-m MODE\n", prog_name);
    printf("        MODE is one of: 'send' or 'receive' (short form 's' or 'r')\n");
}

int parse_args(int argc, char *argv[]) {

    if (argc != 3) {
        puts("Missing (or excessive) arguments.\n");
        print_usage(argv[0]);
        return 1;
    }

    if (!is_one_of(argv[1], OPT_VALUES_MODE)) {
        printf("Unknown first argument '%s'.\n", argv[1]);
        print_usage(argv[0]);
        return 1;
    }

    if (is_one_of(argv[2], OPT_VALUES_RECEIVE)) {
        mostly_receive = true;

    } else if (is_one_of(argv[2], OPT_VALUES_SEND)) {
        mostly_receive = false;

    } else {
        printf("Unknown MODE option '%s'\n", argv[2]);
        print_usage(argv[0]);
        return 1;
    }
    return 0;
}

int test_receive(int times) {
    int fails = 0;
    int successes = 0;
    for (int i = 0; i < times; i++) {
        printf("Receiving frame %d...\n", i + 1);
        funframe_t the_frame;
        payload_t init_values = {
            255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        };

        ff_init(&the_frame, 51, 52, init_values);
        int rc = -1;
        if (rc = ff_receive(&the_frame)) {
            printf(" > Receiving FAILED (rc=%d)\n", rc);
            ff_print(&the_frame);
            fails++;
        } else {
            printf(" > SUCCESS! Received frame: ");
            ff_print(&the_frame);
        };
    }
    successes = times - fails;
    printf("TESTS: %d, SUCCEEDED: %d, FAILED: %d", times, successes, fails);
    return 0;
}

int test_send(int times) {
    payload_t data = {5, 26, 75, 153, 52, 231, 12, 35, 222, 153, 56, 2, 162, 6, 9, 82};

    funframe_t frame1;
    ff_init(&frame1, 41, 42, data);

    printf("Sedning (%d times) frame ", times);
    ff_print(&frame1);
    for (int i = 0; i < times; i++) {
        ff_send(&frame1);
        sleep(300);
    }
    return 0;
}

int main(int argc, char *argv[]) {
    int rc = -1;

    if (rc = parse_args(argc, argv)) {
        return rc;
    }

    printf("Mode: mostly %s.\n", mostly_receive ? "receive" : "send");
    gpio_init(SEND_PIN, RECEIVE_PIN);

    rc = (mostly_receive ? test_receive(COUNT_FRAMES) : test_send(COUNT_FRAMES));

    gpio_cleanup();
    return rc;
}
