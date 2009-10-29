
CC = gcc
CFLAGS = -Wall

OBJ = common.o cassoclist.o deque.o stack.o queue.o \
    test/cassoclist/1.o test/cassoclist/2.o test/deque/1.o test/deque/2.o \
    test/stack/1.o test/stack/2.o test/stack/3.o test/stack/4.o \
    test/queue/1.o test/queue/2.o

BIN = test/cassoclist/1.bin test/cassoclist/2.bin \
    test/deque/1.bin test/deque/2.bin test/stack/1.bin test/stack/2.bin \
    test/stack/3.bin test/stack/4.bin test/queue/1.bin test/queue/2.bin

.SUFFIXES : .c .o .bin

.PHONY : all run clean

.c.o :
	$(CC) $(CFLAGS) -c -o $@ $<

.o.bin :
	$(CC) $(CFLAGS) -o $@ $^

all : $(BIN)

run : $(BIN)
	./test/cassoclist/1.bin
	./test/deque/1.bin
	./test/deque/2.bin
	./test/stack/1.bin
	./test/stack/2.bin
	./test/stack/3.bin
	./test/queue/1.bin
	./test/queue/2.bin

clean : 
	rm $(OBJ) $(BIN)

test/cassoclist/1.bin : test/cassoclist/1.o cassoclist.o common.o
test/cassoclist/2.bin : test/cassoclist/2.o cassoclist.o common.o
test/deque/1.bin : test/deque/1.o deque.o common.o
test/deque/2.bin : test/deque/2.o deque.o common.o
test/stack/1.bin : test/stack/1.o stack.o common.o
test/stack/2.bin : test/stack/2.o stack.o common.o
test/stack/3.bin : test/stack/3.o stack.o common.o
test/stack/4.bin : test/stack/4.o stack.o common.o
test/queue/1.bin : test/queue/1.o queue.o common.o
test/queue/2.bin : test/queue/2.o queue.o common.o

common.o : common.c common_private.h common_public.h
cassoclist.o : cassoclist.c cassoclist.h common_private.h common_public.h
deque.o : deque.c deque.h common_private.h common_public.h
stack.o : stack.c stack.h common_private.h common_public.h
queue.o : queue.c queue.h common_private.h common_public.h
test/cassoclist/1.o : test/cassoclist/1.c cassoclist.h common_public.h
test/cassoclist/2.o : test/cassoclist/2.c cassoclist.h common_public.h
test/deque/1.o : test/deque/1.c deque.h common_public.h
test/deque/2.o : test/deque/2.c deque.h common_public.h
test/stack/1.o : test/stack/1.c stack.h common_public.h
test/stack/2.o : test/stack/2.c stack.h common_public.h
test/stack/3.o : test/stack/3.c stack.h common_public.h
test/stack/4.o : test/stack/4.c stack.h common_public.h
test/queue/1.o : test/queue/1.c queue.h common_public.h
test/queue/2.o : test/queue/2.c queue.h common_public.h

