#include <bcm2835.h>
#include <stdio.h>

#define PIN RPI_GPIO_P1_15

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

int cleanup() {
    int rc = bcm2835_close();
    if (!rc) {
        printf("Clenup FALIED!\n");
    } else {
        printf("Cleaned up - OK\n");
    }
    return !rc;
}

int main(int argc, char const *argv[]) {
    int rc = initialize();
    if (rc)
        return rc;

    printf("Listening on pin %d for debuging...\n", PIN);
    for (int i = 0; i < 200; i++) {
        printf(" %d", bcm2835_gpio_lev(PIN));
        fflush(stdout);
        bcm2835_delay(100);
    }
    printf("\n");
    return cleanup();
}
