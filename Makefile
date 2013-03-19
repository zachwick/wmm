#Basic Makefile for wmm

build: wmm

clean:
	rm -f wmm wmm.o

wmm: wmm.o
	gcc $< -o $@ -lxcb

wmm.o: wmm.c
	gcc -std=gnu99 -c $< -o $@ -lxcb