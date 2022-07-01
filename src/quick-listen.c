#include <bcm2835.h>
#include <stdio.h>

#define PIN RPI_GPIO_P1_15

int main(int argc, char **argv) {

   if (!bcm2835_init()) {
      printf("Initialisation FAILED!\n");
      return 1;
   }

   bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_INPT);
   bcm2835_gpio_set_pad(PIN, BCM2835_GPIO_PUD_DOWN);

   printf("@@@Listening on PIN %u\n", PIN);
   for (int i = 0; i < 80; i++) {
      printf("%u\n", bcm2835_gpio_lev(PIN));
      bcm2835_delay(250);
   }

   bcm2835_close();
   printf("\nCleanup done.\n");
   return 0;
}
