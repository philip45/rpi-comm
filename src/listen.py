#! /usr/bin/env python3

import time
import RPi.GPIO as gpio


class Listener:
    def __init__(self, sync_chan: int, data_chan: int, size: int):
        self._sync_chan: int = sync_chan
        self._data_chan: int = data_chan
        self._size: int = size
        gpio.setmode(gpio.BCM)
        gpio.setup(sync_chan, gpio.IN)
        gpio.setup(data_chan, gpio.IN)
        gpio.add_event_detect(self._sync_chan, gpio.FALLING)

    def listen(self):
        print("Listening...")
        count = 0
        for i in range(5000):
            if gpio.event_detected(self._sync_chan):
                for i in range(self._size):
                    print(
                        gpio.input(self._data_chan),
                    )
                    time.sleep(0.05)

                print()

            time.sleep(0.05)


def main():
    SYNC_CHAN = 25
    DATA_CHAN = 17
    SIZE = 8

    listener = Listener(SYNC_CHAN, DATA_CHAN, SIZE)
    listener.listen()

    gpio.cleanup()


if __name__ == "__main__":
    main()
