#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "../Lib/bcm-ext.h"
#include "../Lib/funnet.h"

#define SEND_PIN 11
#define RECEIVE_PIN 15

bool mostly_receive = 0;

bool is_one_of(char *token, char *options[]) {
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

    if (0 != strcmp("--mode", argv[1])) {
        printf("Unknown first argument '%s'.\n", argv[1]);
        print_usage(argv[0]);
        return 1;
    }

    if (argc != 3) {
        puts("Missing (or excessive) arguments.\n");
        print_usage(argv[0]);
        return 1;
    }

    char *send_values[] = {"send", "s", "@END"};
    char *receive_values[] = {"receive", "r", "listen", "l", "@END"};

    if (is_one_of(argv[2], receive_values)) {
        mostly_receive = true;

    } else if (is_one_of(argv[2], send_values)) {
        mostly_receive = false;

    } else {
        printf("Unknown MODE option '%s'\n", argv[2]);
        print_usage(argv[0]);
        return 1;
    }
    return 0;
}

int main(int argc, char **argv) {
    int rc = -1;

    if (rc = parse_args(argc, argv)) {
        return rc;
    }

    gpio_init(SEND_PIN, RECEIVE_PIN);

    FunFrame frame1;
    payload_t data = {5, 26, 75, 153, 52, 231, 12, 35, 222, 153, 56, 2, 162, 6, 9, 82};
    ff_init(&frame1, 41, 42, data);

    printf("Mostly receive: %d\n", mostly_receive);

    gpio_cleanup();
    return 0;
}
