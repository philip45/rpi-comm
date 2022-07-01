/**
 * @file funnet.c
 * @author Philip Damyanov
 * @author Yassen Damyanov
 * @brief
 * @version 0.0.1
 * @date 2022-06-30
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "funnet.h"
#include <stdio.h>
#include <sys/time.h>

#define US_IN_1SEC 1000000

uint64 absolute(int64 x) {
   if (x < 0) return -x;
   return x;
}

uint64 elapsed_time_us() {
   struct timeval tval;

   gettimeofday(&tval, NULL);
   uint64 useconds = tval.tv_usec;
   return useconds;
}

int32 timediff(uint64 new_stamp, uint64 old_stamp) {
   if (new_stamp >= old_stamp) {
      return new_stamp - old_stamp;
   }
   return US_IN_1SEC - old_stamp + new_stamp;
}

void hello_world() {
   printf("Hello Library World!");
}
