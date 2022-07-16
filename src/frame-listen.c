#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/time.h>

#include <bcm2835.h>

#include "bcm-ext.h"
#include "funnet.h"

// #define US_IN_1SEC 1000000
// #define int32 int_fast32_t
// #define big_uint uint64_tint initialize(uint8 pin);

#define PIN RPI_GPIO_P1_15

const int T = 15000;
const int T_SYNC = 12 * T;
const int T_SCAN = 10;
const int MARGIN = 200;
// const int BIT_DELAY = T * 9970 / 10000; // works with T = 20 000
const int BIT_DELAY = T * 9960 / 10000; // T*9960/10000 works for T = 15 000

// int initialize() {
//     printf("Initializing...\n");

//     if (!bcm2835_init()) {
//         printf("Initialization FAILED!\n");
//         return 1;
//     }

//     bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_INPT);
//     bcm2835_gpio_set_pad(PIN, BCM2835_GPIO_PUD_DOWN);

//     printf("Initialization DONE.\n");
//     return 0;
// }

// big_uint absolute(big_uint x) {
//     if (x < 0) {
//         return -x;
//     }
//     return x;
// }

// big_uint elapsed_time_us() {
//     struct timeval tval;

//     gettimeofday(&tval, NULL);
//     big_uint useconds = tval.tv_usec;
//     return useconds;
// }

// int32 timediff(big_uint new_stamp, big_uint old_stamp) {
//     if (new_stamp >= old_stamp) {
//         return new_stamp - old_stamp;
//     }
//     return US_IN_1SEC - old_stamp + new_stamp;
// }

int wait_for_sync() {
   int level = -1;
   int last_level = -1;
   uint64 rising_stamp = -1;
   uint64 falling_stamp = -1;
   int32 pulse_span = -1;
   bool rising_detected = false;

   printf("Waiting for SYNC...\n\n");

   for (int i = 0; i < 40000; i++) {
      level = bcm2835_gpio_lev(PIN);
      // printf(" %d", level);
      // fflush(stdout);

      if (last_level == LOW && level == HIGH) { // rising
         rising_stamp = elapsed_time_us();
         rising_detected = true;
         // printf("\nRising detected\n");
      } else if (last_level == HIGH && level == LOW) // falling
      {
         // printf("\nFalling detected\n");
         if (!rising_detected) {
            printf("- but NO rising yet! Continue.\n");
            last_level = level;
            continue;
         }

         falling_stamp = elapsed_time_us();
         pulse_span = timediff(falling_stamp, rising_stamp);
         // printf("\nRising stamp=%dl\nFalling stamp=%dl\nPulse span=%dl\n", rising_stamp,
         // falling_stamp, pulse_span);
         if (absolute(pulse_span - T_SYNC) < MARGIN) {
            printf("SYNC ");
            return true;
         } else {
            // printf("\n-- pulse span=%d far from T_SYNC=%d\n", pulse_span, T_SYNC);
         }
      }
      last_level = level;
      bcm2835_delayMicroseconds(T_SCAN);
   }
   printf("wait_for_sync timed out!\n");
   return false;
}

int listen() {
   int last_bit = -2;
   int new_bit = -1;
   int n = 176;

   printf("Listening...\n");

   bool synced = wait_for_sync();
   if (synced) {
      bcm2835_delayMicroseconds(T * 25 / 10);
      for (int i = 0; i < n; i++) {
         new_bit = bcm2835_gpio_lev(PIN);
         printf("%d", new_bit);
         fflush(stdout);
         if (new_bit == last_bit) printf(" Listening failed! ");
         last_bit = new_bit;
         // printf("%d", i);
         bcm2835_delayMicroseconds(BIT_DELAY);
      }
   }
   printf("\n");
   return 0;
}

int main() {
   int rc = -1;
   rc = initialize(PIN, BCM2835_GPIO_FSEL_INPT);
   if (rc) return rc;
   rc = listen();
   rc |= cleanup();
   return rc;
}
