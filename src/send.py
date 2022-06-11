#! /usr/bin/env python3

import time
import atexit

import RPi.GPIO as gpio


class Sender:
    def __init__(self, sync_chan: int, data_chan: int):
        self._sync_chan: int = sync_chan
        self._data_chan: int = data_chan
        gpio.setmode(gpio.BCM)
        gpio.setup(sync_chan, gpio.OUT)
        gpio.output(self._sync_chan, 0)
        gpio.setup(data_chan, gpio.OUT)
        gpio.output(self._data_chan, 0)

    def __del__(self):
        gpio.output(self._sync_chan, 0)
        gpio.output(self._data_chan, 0)
        print("Sender disposed.")

    def _send_sync(self):
        gpio.output(self._sync_chan, 0)
        time.sleep(0.05)
        gpio.output(self._sync_chan, 1)
        time.sleep(0.05)
        gpio.output(self._sync_chan, 0)

    def send(self, b):
        self._send_sync()
        word = f"{b:08b}"
        for ch in word:
            gpio.output(self._data_chan, int(ch))
            print(int(ch))
            time.sleep(0.05)
        print()


def main():
    atexit.register(gpio.cleanup)

    SYNC_CHAN = 22
    DATA_CHAN = 6

    sender = Sender(SYNC_CHAN, DATA_CHAN)
    sender.send(5)
    time.sleep(0.33)
    sender.send(9)
    time.sleep(0.33)
    sender.send(254)


if __name__ == "__main__":
    main()
