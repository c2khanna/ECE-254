# ECE254 lab4 Linux Port
# Makefile: build mqueue sender and receiver example code

CC=gcc
CFLAGS=-Wall -g
LD=gcc
LDFLAGS=-g -lrt


OBJS1=sender.o
OBJS2=receiver.o

all: produce receiver.out

produce: $(OBJS1) 
	$(LD) -o $@ $(OBJS1)  $(LDFLAGS)  

sender.o: sender.c common.h 
	$(CC) $(CFLAGS) -c sender.c 

receiver.out: $(OBJS2) 
	$(LD) -o $@ $(OBJS2)  $(LDFLAGS)  

receiver.o: receiver.c common.h 
	$(CC) $(CFLAGS) -c receiver.c 

.PHONY: clean
clean:
	rm -f *.o *.out produce
