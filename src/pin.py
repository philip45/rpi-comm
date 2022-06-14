#! /usr/bin/env python3

import asyncio

import RPi.GPIO as gpio

sleep = asyncio.sleep


def setup_gpio():
    gpio.setmode(gpio.BCM)


class Pin:
    def __init__(self, bcm_code: int) -> None:
        self.bcm_code = bcm_code
        self.callback_rasing = lambda: None
        self.callback_falling = lambda: None
        self._should_run = False
        # config the pin as input
        gpio.setup(26, gpio.IN)

    def stop(self):
        self._should_run = False

    async def monitor(self):
        old_value = gpio.input(26)
        self._should_run = True
        while self._should_run:
            await asyncio.sleep(0.005)
            new_value = gpio.input(26)
            if old_value == 0 and new_value == 1:
                self.callback_rasing()
            if old_value == 1 and new_value == 0:
                self.callback_falling()
            old_value = new_value


def on_rasing():
    print("Level RAISED!")


def on_falling():
    print("Level FELL!")


async def main():
    setup_gpio()
    p = Pin(26)
    p.callback_rasing = on_rasing
    p.callback_falling = on_falling
    loop = asyncio.get_event_loop()
    loop.create_task(p.monitor())
    await asyncio.sleep(20)


if __name__ == "__main__":
    asyncio.run(main())
