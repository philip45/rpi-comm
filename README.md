# rpi-comm

Raspberry Pi communication for education


## Two-wire unidirectional comm

We first got a uni-direction unidirectional with two wires working.
One wire was for synchronization and the other was for the data.

This was done only in Python.

## One-wire unidirectional comm

After a while, we developped a single-wire unidirectional communication
porotocol as follows:

* We choose a time span T;
* We encode a sync pulse as HIGH level 3*T/2;
* We encode a bit ONE with 2*T/3;
* We encode a bit ZERO with 1*T/3.

The listener follows the level changes and detects a sync pulse,
then ones and zeros.

The above was implemented first in Python, then C lang.

## Prerequisites

- gcc
- bcm2835 library installed - https://www.airspayce.com/mikem/bcm2835/

## How to install bcm2835 library

* Download latest version -- the link is in the library home page;
* Install the dependencies (see below);
* Untar it and go into its directory;
* Execute the `./configure` script;
* Compile with `make`;
* Run tests with `sudo make check`;
* On sucess of the above -- install with `sudo make install`.

On Raspbian light 11.3 64bit with kernel Linux raspberrypi 5.15.32-v8+ 2022
aarch64 GNU/Linux, the commands look like this:


```bash
apt-get install libcap2 libcap-dev

# Recommended lib libc6-dev was already there;
# these two: libgcc-s-dev libstdc++-staticdev were not found anyway

mkdir ~/src; cd ~/src
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz
tar xf bcm2835-1.71.tar.gz
./configure
make
sudo make check
sudo make install

```
Enjoy! ;)
