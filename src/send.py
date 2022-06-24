#! /usr/bin/env python3

import time
import atexit

import RPi.GPIO as gpio


T = 0.015


class Sender:
    def __init__(self, data_chan: int):
        #self._sync_chan: int = sync_chan
        self._data_chan: int = data_chan
        gpio.setmode(gpio.BCM)
        #gpio.setup(sync_chan, gpio.OUT)
        #gpio.output(self._sync_chan, 0)
        gpio.setup(data_chan, gpio.OUT)
        gpio.output(self._data_chan, 0)

    def __del__(self):
        #gpio.output(self._sync_chan, 0)
        gpio.output(self._data_chan, 0)
        print("Sender disposed.")

    def _send_sync(self):
        gpio.output(self._sync_chan, 0)
        time.sleep(0.05)
        gpio.output(self._sync_chan, 1)
        time.sleep(0.05)
        gpio.output(self._sync_chan, 0)

    # def send(self, b):
    #     self._send_sync()
    #     word = f"{b:08b}"
    #     for ch in word:
    #         gpio.output(self._data_chan, int(ch))
    #         print(int(ch))
    #         time.sleep(0.05)
    #     print()

    def send_bit(self, ch):
        if int(ch) == 0:
            gpio.output(26, gpio.HIGH)
            time.sleep(T/3)
            gpio.output(26, gpio.LOW)
            time.sleep(2*T/3)
        elif int(ch) == 1:
            gpio.output(26, gpio.HIGH)
            time.sleep(2*T/3)
            gpio.output(26, gpio.LOW)
            time.sleep(T/3)
        else:
            raise RuntimeError("Unreachable")
        
    def send_sync(self):
        gpio.output(26, gpio.HIGH)
        time.sleep(1.5*T)
        gpio.output(26, gpio.LOW)
        time.sleep(T/2)

    def send(self, b):
        self.send_sync()
        word = f"{b:08b}"
        for ch in word:
            self.send_bit(ch)
        # print()

    def send_str(self, s: str):
        for ch in s:
            self.send(ord(ch))


def main():
    atexit.register(gpio.cleanup)

    # SYNC_CHAN = 22
    DATA_CHAN = 26

    s = input("Enter text: ")
    sender = Sender(DATA_CHAN)
    sender.send_str(f"{s}\x0b ")
    # sender.send(5)
    # sender.send(9)
    # sender.send(100)
    # sender.send(254)
    # sender.send(5)
    # sender.send(9)
    # sender.send(100)
    # sender.send(254)
    # sender.send(5)
    # sender.send(9)
    # sender.send(100)
    # sender.send(254)


if __name__ == "__main__":
    main()
