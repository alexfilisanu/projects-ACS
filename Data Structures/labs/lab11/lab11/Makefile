CC = gcc
CFLAGS = -Wall -g

build: heap

heap: heap.c 
	$(CC) $(CFLAGS) $^ -o $@

run:
	./heap

.PHONY: clean
clean:
	rm -f *.o *~ heap
