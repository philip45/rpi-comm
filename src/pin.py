#! /usr/bin/env python3

import asyncio
import time

import RPi.GPIO as gpio

sleep = asyncio.sleep
T = 0.015
MARGIN = T/6

buffer = ""


def setup_gpio():
    gpio.setmode(gpio.BCM)


class Pin:
    def __init__(self, bcm_code: int) -> None:
        self.bcm_code = bcm_code
        self.callback_rasing = lambda: None
        self.callback_falling = lambda: None
        self._should_run = False
        # config the pin as input
        gpio.setup(16, gpio.IN)

    def stop(self):
        self._should_run = False

    # async def monitor(self):
    #     old_value = gpio.input(16)
    #     self._should_run = True
    #     start_time = time.time()
    #     while self._should_run:
    #         await asyncio.sleep(0.005)
    #         new_value = gpio.input(16)
    #         if old_value == 0 and new_value == 1:
    #             now = time.time()
    #             timestamp = now - start_time
    #             start_time = now
    #             self.callback_rasing(timestamp)
    #         if old_value == 1 and new_value == 0:
    #             now = time.time()
    #             timestamp = now - start_time
    #             start_time = now
    #             self.callback_falling(timestamp)
    #         old_value = new_value

    async def monitor(self):
        old_value = gpio.input(16)
        self._should_run = True
        start_time = time.time()
        while self._should_run:
            await asyncio.sleep(0.0001)
            new_value = gpio.input(16)
            if old_value == 0 and new_value == 1:
                now = time.time()
                timestamp = now - start_time
                start_time = now
                self.callback_rasing(timestamp)
            if old_value == 1 and new_value == 0:
                now = time.time()
                timestamp = now - start_time
                start_time = now
                self.callback_falling(timestamp)
            old_value = new_value



def on_rasing(timestamp):
    # print("Level RAISED! timestamp:{}".format(timestamp))
    pass

def on_falling(timestamp):
    global buffer
    # print("Level FELL! timestamp:{}".format(timestamp))
    time_0 = T/3
    time_1 = 2*T/3
    time_sync = 1.5*T

    if abs(timestamp - time_0) < MARGIN:
        # print(0)
        buffer += "0"
    elif abs(timestamp - time_1) < MARGIN:
        buffer += "1"
        # print(1)
    elif abs(timestamp - time_sync) < MARGIN:
        if buffer:
            ch = "".join(chr(int(buffer, 2)))
            if ch == '\x0b':
                print()
            else:
                print(ch, end="")
            buffer = ""
        # print("sync")
    else:
        print("unknown signal; timestamp={}".format(timestamp))


async def main():
    setup_gpio()
    p = Pin(16)
    p.callback_rasing = on_rasing
    p.callback_falling = on_falling
    loop = asyncio.get_event_loop()
    loop.create_task(p.monitor())
    while True:
        await asyncio.sleep(1)


if __name__ == "__main__":
    asyncio.run(main())
