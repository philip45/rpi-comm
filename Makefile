# Funnet project Makefile

LIBS = Lib/libfunnet.a Lib/libbcm-ext.a

all: listen try


Lib/libfunnet.a: Lib/funnet.c Lib/funnet.h
	gcc -c Lib/funnet.c -o Lib/libfunnet.a

Lib/libbcm-ext.a: Lib/bcm-ext.c Lib/bcm-ext.h
	gcc -c Lib/bcm-ext.c -o Lib/libbcm-ext.a


libs: $(LIBS)


build/debug/try: src/try.c Lib/libfunnet.a
	gcc src/try.c -o build/debug/try -I Lib -L Lib -l funnet

try: build/debug/try

build/debug/frame-listen: src/frame-listen.c $(LIBS)
	gcc src/frame-listen.c -o build/debug/frame-listen -I Lib -L Lib -l bcm2835 -l funnet -l bcm-ext

frame-listen: build/debug/frame-listen

build/debug/frame-send-new: src/frame-send-new.c $(LIBS)
	gcc src/frame-send-new.c -o build/debug/frame-send-new -I Lib -L Lib -l bcm2835 -l funnet -l bcm-ext

frame-send-new: build/debug/frame-send-new

build/debug/frame-send: src/frame-send.c Lib/libbcm-ext.a
	gcc src/frame-send.c -o build/debug/frame-send -I Lib -L Lib -l bcm2835 -l bcm-ext

frame-send: build/debug/frame-send

clean:
	rm -f build/debug/try

clean-all:
	rm -f build/debug/*
	rm -f build/release/*
	rm -f Lib/*.a

.PHONY: clean clean-all
