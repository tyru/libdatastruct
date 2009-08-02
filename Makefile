
all : run

bin : bin/assoclist/1 bin/assoclist/2 bin/deque/1 bin/deque/2 \
    bin/stack/1 bin/stack/2 bin/stack/3 bin/queue/1 bin/queue/2

obj/ :
	mkdir -p obj/

obj/libdatastruct.o : src/libdatastruct.c src/libdatastruct.h obj/
	gcc -o obj/libdatastruct.o -c src/libdatastruct.c

obj/assoclist.o : src/assoclist.c src/libdatastruct.h obj/
	gcc -o obj/assoclist.o -c src/assoclist.c

obj/deque.o : src/deque.c src/libdatastruct.h obj/
	gcc -o obj/deque.o -c src/deque.c

obj/stack.o : src/stack.c src/libdatastruct.h obj/
	gcc -o obj/stack.o -c src/stack.c

obj/queue.o : src/queue.c src/libdatastruct.h obj/
	gcc -o obj/queue.o -c src/queue.c

obj/test/assoclist/1.o : src/test/assoclist/1.c src/libdatastruct.h
	mkdir -p obj/test/assoclist/
	gcc -o $@ -c src/test/assoclist/1.c

obj/test/assoclist/2.o : src/test/assoclist/2.c src/libdatastruct.h
	mkdir -p obj/test/assoclist/
	gcc -o $@ -c src/test/assoclist/2.c

obj/test/deque/1.o : src/test/deque/1.c src/libdatastruct.h
	mkdir -p obj/test/deque/
	gcc -o $@ -c src/test/deque/1.c

obj/test/deque/2.o : src/test/deque/2.c src/libdatastruct.h
	mkdir -p obj/test/deque/
	gcc -o $@ -c src/test/deque/2.c

obj/test/stack/1.o : src/test/stack/1.c src/libdatastruct.h
	mkdir -p obj/test/stack/
	gcc -o $@ -c src/test/stack/1.c

obj/test/stack/2.o : src/test/stack/2.c src/libdatastruct.h
	mkdir -p obj/test/stack/
	gcc -o $@ -c src/test/stack/2.c

obj/test/stack/3.o : src/test/stack/3.c src/libdatastruct.h
	mkdir -p obj/test/stack/
	gcc -o $@ -c src/test/stack/3.c

obj/test/queue/1.o : src/test/queue/1.c src/libdatastruct.h
	mkdir -p obj/test/queue/
	gcc -o $@ -c src/test/queue/1.c

obj/test/queue/2.o : src/test/queue/2.c src/libdatastruct.h
	mkdir -p obj/test/queue/
	gcc -o $@ -c src/test/queue/2.c

bin/assoclist/1 : obj/libdatastruct.o obj/assoclist.o obj/test/assoclist/1.o
	mkdir -p bin/assoclist/
	gcc -o $@ $^

bin/assoclist/2 : obj/libdatastruct.o obj/assoclist.o obj/test/assoclist/2.o
	mkdir -p bin/assoclist/
	gcc -o $@ $^

bin/deque/1 : obj/libdatastruct.o obj/deque.o obj/test/deque/1.o
	mkdir -p bin/deque/
	gcc -o $@ $^

bin/deque/2 : obj/libdatastruct.o obj/deque.o obj/test/deque/2.o
	mkdir -p bin/deque/
	gcc -o $@ $^

bin/stack/1 : obj/libdatastruct.o obj/stack.o obj/test/stack/1.o
	mkdir -p bin/stack/
	gcc -o $@ $^

bin/stack/2 : obj/libdatastruct.o obj/stack.o obj/test/stack/2.o
	mkdir -p bin/stack/
	gcc -o $@ $^

bin/stack/3 : obj/libdatastruct.o obj/stack.o obj/test/stack/3.o
	mkdir -p bin/stack/
	gcc -o $@ $^

bin/queue/1 : obj/libdatastruct.o obj/queue.o obj/test/queue/1.o
	mkdir -p bin/queue/
	gcc -o $@ $^

bin/queue/2 : obj/libdatastruct.o obj/queue.o obj/test/queue/2.o
	mkdir -p bin/queue/
	gcc -o $@ $^

run : bin
	@echo
	@echo bin/assoclist/1
	@echo
	@bin/assoclist/1
	
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
