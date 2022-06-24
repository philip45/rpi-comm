#include <stdio.h>
#include <bcm2835.h>

#define PIN RPI_GPIO_P1_11

int main(int argc, char **argv)
{
    // If you call this, it will not actually access the GPIO
    // Use for testing
//    bcm2835_set_debug(1);

    if (!bcm2835_init()) {
        printf("Initialisation FAILED!\n");
        return 1;
    }

    // Set RPI pin P1-11 to be an input
    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_INPT);
    // bcm2835_gpio_set_pud(PIN, BCM2835_GPIO_PUD_UP);
    //  with a pull-down
    bcm2835_gpio_set_pad(PIN, BCM2835_GPIO_PUD_DOWN);
    // And a low detect enable

    bcm2835_gpio_len(PIN);
    bcm2835_gpio_hen(PIN);

    printf("Listening on PIN %u\n", PIN);
    while (1)
    {
        if (bcm2835_gpio_eds(PIN)) {
            // Now clear the eds flag by setting it to 1
            bcm2835_gpio_set_eds(PIN);
            printf("\r%u ", bcm2835_gpio_lev(PIN));
        }
        else { 
            printf("\r+++++");
        }
        // wait a bit
        delay(500);
        printf("\r-----");
    }

    bcm2835_gpio_clr_len(PIN);
    bcm2835_gpio_clr_hen(PIN);
    bcm2835_close();
    printf("\nCleanup done.\n");
    return 0;
}