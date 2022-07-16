# Funnet project Makefile

LIBS = Lib/libfunnet.a Lib/libbcm-ext.a

all: libs ff-comm


Lib/libbcm-ext.a: Lib/bcm-ext.c Lib/bcm-ext.h
	gcc -c Lib/bcm-ext.c -o Lib/libbcm-ext.a


Lib/libfunnet.a: Lib/funnet.c Lib/funnet.h Lib/libbcm-ext.a
	gcc -c Lib/funnet.c -o Lib/libfunnet.a

libs: $(LIBS)

build/debug/frame-listen: src/frame-listen.c $(LIBS)
	gcc src/frame-listen.c -o build/debug/frame-listen -I Lib -L Lib -l bcm2835 -l funnet -l bcm-ext

frame-listen: build/debug/frame-listen

build/debug/frame-send-new: src/frame-send-new.c $(LIBS)
	gcc src/frame-send-new.c -o build/debug/frame-send-new -I Lib -L Lib -l bcm2835 -l funnet -l bcm-ext

frame-send-new: build/debug/frame-send-new

build/debug/frame-send: src/frame-send.c Lib/libbcm-ext.a
	gcc src/frame-send.c -o build/debug/frame-send -I Lib -L Lib -l bcm2835 -l bcm-ext

frame-send: build/debug/frame-send

# build/debug/ff-comm: src/ff-comm.c $(LIBS)
# 	gcc src/ff-comm.c -o build/debug/ff-comm -I Lib -L Lib -l bcm2835 -l funnet -l bcm-ext

build/debug/ff-comm: src/ff-comm.c Lib/bcm-ext.h Lib/bcm-ext.c Lib/funnet.h Lib/funnet.c
	gcc src/ff-comm.c -o build/debug/ff-comm Lib/bcm-ext.c Lib/funnet.c -l bcm2835

ff-comm: build/debug/ff-comm


clean:
	rm -f build/debug/*

clean-all:
	rm -f build/debug/*
	rm -f build/release/*
	rm -f Lib/*.a

.PHONY: clean clean-all
