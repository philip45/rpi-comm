#include <inttypes.h>
#include <stdio.h>

#include <bcm2835.h>

#include "bcm-ext.h"

#define PIN RPI_GPIO_P1_11

#define uint8 uint8_t
#define big_uint uint64_t

/**
 * bcm2835_delayMicroseconds() fails for args larger than 166666
 *  - makes the delay much shorter than it should be.
 */

const big_uint T = 166666; // microseconds
const big_uint T_ZERO_ON = T / 3;
const big_uint T_ZERO_OFF = 2 * T / 3;
const big_uint T_ONE_ON = 2 * T / 3;
const big_uint T_ONE_OFF = T / 3;
const big_uint T_SYNC_ON = 3 * T / 2;
const big_uint T_SYNC_OFF = T / 2;

void send_pulse(big_uint time_on, big_uint time_off) {
   printf("%d (%d)\n", HIGH, time_on);
   bcm2835_gpio_write(PIN, HIGH);
   bcm2835_delayMicroseconds((uint64_t)time_on);

   printf("%d (%d)\n", LOW, time_off);
   bcm2835_gpio_write(PIN, LOW);
   bcm2835_delayMicroseconds((uint64_t)time_off);
}

void send_sync() {
   printf("BEGIN SYNC =======\n");
   printf("%d (%d) (preliminary)\n", LOW, T_SYNC_OFF);
   bcm2835_gpio_write(PIN, LOW);
   bcm2835_delayMicroseconds((uint64_t)T_SYNC_OFF);

   send_pulse(T_SYNC_ON, T_SYNC_OFF);
   printf("END SYNC =======\n");
}

void send_zero() {
   printf("Send ZERO -----\n");
   send_pulse(T_ZERO_ON, T_ZERO_OFF);
}

void send_one() {
   printf("Send ONE +++++\n");
   send_pulse(T_ONE_ON, T_ONE_OFF);
}

void send_byte(uint8 byte) {
   send_sync();

   for (uint8 mask = 0x80; mask != 0; mask >>= 1) {
      if (byte & mask) {
         send_one();
      } else {
         send_zero();
      }
   }
}

int main(/*int argc, char *argv[]*/) {
   // if (argc != 2) {
   //     printf("Wrong number of arguments(%u)", argc);
   //     printf("Usage:  send {MESSAGE-TO-BE-SENT}");
   //     return 1;
   // }

   initialize(PIN, BCM2835_GPIO_FSEL_OUTP);

   printf("Sending first group...\n");

   send_byte(3);
   send_byte(0);
   send_byte(204);
   send_byte(170);
   send_byte(11);
   send_byte(17);
   send_byte(42);
   send_byte(170);
   send_byte(9);
   send_byte(5);
   send_byte(5);
   send_byte(9);
   send_byte(254);
   send_byte(5);
   send_byte(9);
   send_byte(254);
   send_byte(5);
   send_byte(9);
   send_byte(0);
   send_byte(254);

   // send_byte(9);
   // send_byte(254);

   // printf("Sending second group...\n");

   // send_byte(5);
   // send_byte(9);
   // send_byte(254);

   printf("DONE.\n");
   return 0;
}
