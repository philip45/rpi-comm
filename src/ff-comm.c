#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../Lib/bcm-ext.h"
#include "../Lib/funnet.h"

#define SEND_PIN 17
#define RECEIVE_PIN 22

const char *OPT_VALUES_SEND[] = {"send", "s", "@END"};
const char *OPT_VALUES_RECEIVE[] = {"receive", "r", "listen", "l", "@END"};
const char *OPT_VALUES_MODE[] = {"--mode", "-m", "@END"};

bool mostly_receive = 0;

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
    printf("USAGE:  %s --mode MODE\n", prog_name);
    printf("        MODE is one of: 'send' or 'receive' (short form 's' or 'r')\n");
}

int parse_args(int argc, char **argv) {

    if (!is_one_of(argv[1], OPT_VALUES_MODE)) {
        printf("Unknown first argument '%s'.\n", argv[1]);
        print_usage(argv[0]);
        return 1;
    }

    if (argc != 3) {
        puts("Missing (or excessive) arguments.\n");
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

void cleanup() {
    gpio_cleanup();
}

int main(int argc, char **argv) {
    int rc = -1;

    if (rc = parse_args(argc, argv)) {
        return rc;
    }

    printf("Mode: mostly %s.\n", mostly_receive ? "receive" : "send");
    gpio_init(SEND_PIN, RECEIVE_PIN);

    if (mostly_receive) {
        printf("Receiving...\n");
        FunFrame the_frame;
        payload_t init_values = {
            255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        };

        ff_init(&the_frame, 51, 52, init_values);
        if (rc = ff_receive(&the_frame)) {
            printf("Receiving FAILED (rc=%d)\n", rc);
            ff_print(&the_frame);
            gpio_cleanup();
            return rc;
        };

        printf("SUCCESS! Received frame: ");
        ff_print(&the_frame);

    } else { // i.e. mostly *send*
        payload_t data = {5, 26, 75, 153, 52, 231, 12, 35, 222, 153, 56, 2, 162, 6, 9, 82};

        FunFrame frame1;
        ff_init(&frame1, 41, 42, data);

        printf("Sedning frame ");
        ff_print(&frame1);
        ff_send(&frame1);
    }

    gpio_cleanup();
    return 0;
}
