
CC = gcc

all : run

bin : bin/cassoclist/1 bin/cassoclist/2 bin/deque/1 bin/deque/2 \
    bin/stack/1 bin/stack/2 bin/stack/3 bin/stack/4 \
	bin/queue/1 bin/queue/2

obj/cassoclist.o : src/cassoclist.c src/cassoclist.h
	mkdir -p obj
	$(CC) -o $@ -c src/cassoclist.c

obj/deque.o : src/deque.c src/cassoclist.h
	mkdir -p obj
	$(CC) -o $@ -c src/deque.c

obj/stack.o : src/stack.c src/stack.h
	mkdir -p obj
	$(CC) -o $@ -c src/stack.c

obj/queue.o : src/queue.c src/queue.h
	mkdir -p obj
	$(CC) -o $@ -c src/queue.c

obj/test/cassoclist/1.o : src/test/cassoclist/1.c src/cassoclist.h
	mkdir -p obj/test/cassoclist/
	$(CC) -o $@ -c src/test/cassoclist/1.c

obj/test/cassoclist/2.o : src/test/cassoclist/2.c src/cassoclist.h
	mkdir -p obj/test/cassoclist/
	$(CC) -o $@ -c src/test/cassoclist/2.c

obj/test/deque/1.o : src/test/deque/1.c src/deque.h
	mkdir -p obj/test/deque/
	$(CC) -o $@ -c src/test/deque/1.c

obj/test/deque/2.o : src/test/deque/2.c src/deque.h
	mkdir -p obj/test/deque/
	$(CC) -o $@ -c src/test/deque/2.c

obj/test/stack/1.o : src/test/stack/1.c src/stack.h
	mkdir -p obj/test/stack/
	$(CC) -o $@ -c src/test/stack/1.c

obj/test/stack/2.o : src/test/stack/2.c src/stack.h
	mkdir -p obj/test/stack/
	$(CC) -o $@ -c src/test/stack/2.c

obj/test/stack/3.o : src/test/stack/3.c src/stack.h
	mkdir -p obj/test/stack/
	$(CC) -o $@ -c src/test/stack/3.c

obj/test/stack/4.o : src/test/stack/4.c src/stack.h
	mkdir -p obj/test/stack/
	$(CC) -o $@ -c src/test/stack/4.c

obj/test/queue/1.o : src/test/queue/1.c src/queue.h
	mkdir -p obj/test/queue/
	$(CC) -o $@ -c src/test/queue/1.c

obj/test/queue/2.o : src/test/queue/2.c src/queue.h
	mkdir -p obj/test/queue/
	$(CC) -o $@ -c src/test/queue/2.c

bin/cassoclist/1 : obj/cassoclist.o obj/test/cassoclist/1.o
	mkdir -p bin/cassoclist/
	$(CC) -o $@ $^

bin/cassoclist/2 : obj/cassoclist.o obj/test/cassoclist/2.o
	mkdir -p bin/cassoclist/
	$(CC) -o $@ $^

bin/deque/1 : obj/deque.o obj/test/deque/1.o
	mkdir -p bin/deque/
	$(CC) -o $@ $^

bin/deque/2 : obj/deque.o obj/test/deque/2.o
	mkdir -p bin/deque/
	$(CC) -o $@ $^

bin/stack/1 : obj/stack.o obj/test/stack/1.o
	mkdir -p bin/stack/
	$(CC) -o $@ $^

bin/stack/2 : obj/stack.o obj/test/stack/2.o
	mkdir -p bin/stack/
	$(CC) -o $@ $^

bin/stack/3 : obj/stack.o obj/test/stack/3.o
	mkdir -p bin/stack/
	$(CC) -o $@ $^

bin/stack/4 : obj/stack.o obj/test/stack/4.o
	mkdir -p bin/stack/
	$(CC) -o $@ $^

bin/queue/1 : obj/queue.o obj/test/queue/1.o
	mkdir -p bin/queue/
	$(CC) -o $@ $^

bin/queue/2 : obj/queue.o obj/test/queue/2.o
	mkdir -p bin/queue/
	$(CC) -o $@ $^

run : bin
	@echo
	@echo bin/cassoclist/1
	@echo
	@bin/cassoclist/1
	
	@echo
	@echo bin/deque/1
	@echo
	@bin/deque/1
	
	@echo
	@echo bin/deque/2
	@echo
	@bin/deque/2
	
	@echo
	@echo bin/stack/1
	@echo
	@bin/stack/1
	
	@echo
	@echo bin/stack/2
	@echo
	@bin/stack/2
	
	@echo
	@echo bin/stack/4
	@echo
	@bin/stack/4
	
	@echo
	@echo bin/queue/1
	@echo
	@bin/queue/1
	
	@echo
	@echo bin/queue/2
	@echo
	@bin/queue/2

clean :
	rm -rf obj
	rm -rf bin
