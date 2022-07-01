#include <stdio.h>
#include <bcm2835.h>
#include <time.h>
#include <sys/time.h>
#include <stdbool.h>

#define PIN RPI_GPIO_P1_15

const int T = 1000;
const int T_ZERO = T / 3;
const int T_ONE = 2 * T / 3;
const int T_SYNC = 3 * T / 2;
const int T_SCAN = T / 10;
const int MARGIN = 2 * T_SCAN;
// double time_high = 0;


long wall_time_us() {
    struct timeval tval;
    gettimeofday(&tval, NULL);
    long useconds = tval.tv_usec + tval.tv_sec * 1000000;
    return useconds;
}


int elapsed_time_us() {
    struct timeval tval;
    gettimeofday(&tval, NULL);
    int useconds = tval.tv_usec;
    return useconds;
}

int timediff(int new_stamp, int old_stamp) {
    if (new_stamp >= old_stamp){
        return new_stamp - old_stamp;
    }
    return 1000000 - old_stamp + new_stamp;
}

int initialization()
{
    printf("Initializing...\n");
    if (!bcm2835_init())
    {
        printf("Initialization FAILED!\n");
        return 1;
    }
    bcm2835_gpio_fsel(PIN, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pad(PIN, BCM2835_GPIO_PUD_DOWN);
    // bcm2835_gpio_len(PIN);init
    // bcm2835_gpio_hen(PIN);
    printf("Initialization DONE.\n");
    return 0;
}

int bit_recogniser(int pulse_span)
{
    if ((pulse_span - T_ZERO) < MARGIN) return 0;
    if ((pulse_span - T_ONE) < MARGIN) return 1;
    if ((pulse_span - T_SYNC) < MARGIN) return 10;
    return -99;
}

int listen_2()
{

    // bool should_run = true;

    // clock_t l2h_timestamp;
    // clock_t h2l_timestamp;
    int l2h_stamp, h2l_stamp;
    int l2h_detected = false;
    int pulse_span;
    long benchmark_start_time = -1;

    int level = -1;
    int last_level = -1;
    int bit_count = 0;
    int byte_count = 0;

    while (true)
    {
        level = bcm2835_gpio_lev(PIN);

        if (last_level == LOW && level == HIGH) // l2h
        {
            l2h_stamp = elapsed_time_us();
            l2h_detected = true;
            benchmark_start_time = wall_time_us();
        }
        else if (last_level == HIGH && level == LOW) // h2l
        {
            if (!l2h_detected)  continue;
            h2l_stamp = elapsed_time_us();
            pulse_span = timediff(h2l_stamp, l2h_stamp);
            int data_bit = bit_recogniser(pulse_span);
            switch (data_bit)
            {
            case 0:
                printf("0");
                bit_count++;
                break;
            case 1:
                printf("1");
                bit_count++;
                break;
            case 10:
                printf("SYNC ");
                break;
            default:
                printf("Unrecognized signal.");
                break;
            }
            if (bit_count == 8){
                printf("\n");
                byte_count++;
                bit_count = 0;
            }
            if (byte_count == 18) {
                printf("Total time (us): %d\n", wall_time_us() - benchmark_start_time);
                break;
            }
        }

        last_level = level;

        // wait for change LOW->HIGH
        // snapshot the time
        // wait for change HIGH->LOW
        // compute the time
        // check the time
    }
}

// int listen_1()
// {
//     int count;
//     printf("Listening...\n");
//     while (1)
//     {
//         if (bcm2835_gpio_lev(PIN) == HIGH)
//         {
//             clock_t t;
//             t = clock();
//             while (1)
//             {
//                 if (bcm2835_gpio_lev(PIN) == LOW)
//                 {
//                     t = clock() - t;
//                     time_high = ((double)t) / CLOCKS_PER_SEC;
//                     bit_recogniser(time_high);
//                     count++;
//                     break;
//                 }
//                 else
//                 {
//                     delay(T_SCAN);
//                 }
//             }
//         }
//         else if (count == 16)
//         {
//             break;
//         }
//         else
//         {
//             delay(T_SCAN);
//         }
//     }
//     return 0;
// }

int main()
{
    initialization();
    listen_2();
    bcm2835_close();
    printf("DONE.\n");
    return 0;
}
