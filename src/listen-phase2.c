#include <stdio.h>
#include <bcm2835.h>
#include <sys/time.h>
#include <stdbool.h>

#define int32 __int32_t

#define PIN RPI_GPIO_P1_15

const int32 T = 166666;
const int32 T_ZERO = T/3;
const int32 T_ONE = 2 * T/3;
const int32 T_SYNC = 3 * T/2;
const int32 T_SCAN = T/10;
const int32 MARGIN = 2 * T_SCAN;
const int32 SMALL_DELAY = T/100;


int absolute(int x) {
    if (x < 0) {
        return -x;
    }
    return x;
}

int initialize() {
    printf("Initializing...\n");

    if (!bcm2835_init()) {
        printf("Initialization FAILED!\n");
        return 1;
    }

    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pad(PIN, BCM2835_GPIO_PUD_DOWN);

    printf("Initialization DONE.\n");
    return 0;
}

int32 elapsed_time_us() {
    struct timeval tval;

    gettimeofday(&tval, NULL);
    int useconds = tval.tv_usec;
    return useconds;
}

int32 timediff(int32 new_stamp, int32 old_stamp) {
    if (new_stamp >= old_stamp) {
        return new_stamp - old_stamp;
    }
    return 1000000 - old_stamp + new_stamp;
}

/**
 * Block Waiting until a sync pulse is detected and return
 * zero immediatelty. Return 1 on timeout.
 */
int wait_for_sync() {
    int level = -1;
    int last_level = -1;
    int32 rising_stamp = -1;
    int32 falling_stamp = -1;
    int pulse_span = -1;
    bool rising_detected = false;

    printf("Waiting for SYNC...\n");

    /**
     * If SMALL_DELAY is OFF, i<350000000 =~ 30 seconds.
     * If SMALL_DELAY is ON, i<8000 =~ 14 seconds.
     */
    for (int i=0; i<8000; i++) {
        level = bcm2835_gpio_lev(PIN);
        // printf(" Level -> %d\n", level);

        if (last_level == LOW && level == HIGH) { // rising
            rising_stamp = elapsed_time_us();
            rising_detected = true;
            printf("Rising detected\n");
        }
        else if (last_level == HIGH && level == LOW) // falling
        {
            printf("Falling detected\n");
            if (!rising_detected) {
                printf("- but NO rising yet! Continue.\n");
                last_level = level;
                continue;
            }

            falling_stamp = elapsed_time_us();
            pulse_span = timediff(falling_stamp, rising_stamp);
            if (absolute(pulse_span - T_SYNC) < MARGIN) {
                printf("SYNC detected\n");
                return 0;
            }
        }
        last_level = level;
        bcm2835_delayMicroseconds(SMALL_DELAY);
    }
    printf("wait_for_sync timed out!\n");
    return 1;
}

int listen() {
    printf("Listening...\n");
    int rc = -1;

    for (int i=0; i<20; i++) {
        rc = wait_for_sync();
        if (rc) return rc;

        // bcm2835_delayMicroseconds(T - T/8);
        // printf("%d\n", bcm2835_gpio_lev(PIN));
        for (int i=0; i<8; i++) {
            bcm2835_delayMicroseconds(T);
            printf("%d", bcm2835_gpio_lev(PIN));
        }
        printf("\n");
    }
    return 0;
}


int main() {
    int rc = -1;

    rc = initialize();
    if (rc) return rc;
    rc = listen();

    bcm2835_close();
    if (rc) printf("Listen timed out.\n");
    else printf("DONE.\n");
    return rc;
}
