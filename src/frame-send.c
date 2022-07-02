#include <stdio.h>

#include <bcm2835.h>

#include "bcm-ext.h"

#define PIN RPI_GPIO_P1_11

const int T = 15000; // microseconds
const int T_SYNC_ON = 12 * T;
const int T_SYNC_OFF = 2 * T;

void send_sync_sig() {
   printf("Sending SYNC signal...\n");
   bcm2835_gpio_write(PIN, LOW);
   bcm2835_delayMicroseconds(T_SYNC_OFF);
   bcm2835_gpio_write(PIN, HIGH);
   bcm2835_delayMicroseconds(T_SYNC_ON);

   bcm2835_gpio_write(PIN, LOW);
   bcm2835_delayMicroseconds(T_SYNC_OFF);
}

int send_bit(int lvl) {
   if (lvl == 1) {
      bcm2835_gpio_write(PIN, HIGH);
      bcm2835_delayMicroseconds(T);
      bcm2835_gpio_write(PIN, LOW);
   } else {
      bcm2835_gpio_write(PIN, LOW);
      bcm2835_delayMicroseconds(T);
   }
   return 0;
}

void send_data(int pairs) {
   for (int i = 0; i < pairs; i++) {
      send_bit(1);
      send_bit(0);
   }
}

int main() {
   initialize(PIN, BCM2835_GPIO_FSEL_OUTP);
   send_sync_sig();
   printf("Sending data...\n");
   send_data(88);
}