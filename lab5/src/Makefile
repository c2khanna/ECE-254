# ECE254 lab4 Linux Port
# Makefile: build mqueue sender and receiver example code

CC=gcc
CFLAGS=-Wall -g
LD=gcc
LDFLAGS=-g -lrt


OBJS1=producer.o
OBJS2=main.o
OBJS3=consumer.o

all: produce producer.out consumer.out

producer.out: $(OBJS1) 
	$(LD) -o $@ $(OBJS1)  $(LDFLAGS)  

producer.o: producer.c 
	$(CC) $(CFLAGS) -c  producer.c 

produce: $(OBJS2) 
	$(LD) -o $@ $(OBJS2)  $(LDFLAGS) -lm  

main.o: main.c 
	$(CC) $(CFLAGS) -c main.c

consumer.out: $(OBJS3)
	$(LD) -o $@ $(OBJS3)  $(LDFLAGS) -lm

consumer.o: consumer.c
	$(CC) $(CFLAGS) -c consumer.c 

.PHONY: clean
clean:
	rm -f *.o *.out produce
