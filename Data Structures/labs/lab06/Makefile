CC = gcc
CFLAGS = -Wall -g

all: main

main: functiiCoada-V.o functiiStiva-V.o main.o
	$(CC) $(CFLAGS) $^ -o $@

main.o: main.c TCoada.h TStiva.h
	$(CC) $(CFLAGS) -c $< -o $@

functiiCoada-V.o: functiiCoada-V.c TCoada.h TCoada-V.h
	$(CC) $(CFLAGS) -c $< -o $@

functiiStiva-V.o: functiiStiva-V.c TStiva.h TStiva-V.h
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean checkMem

run:
	./main

clean:
	rm -f *.o main

checkMem:
	valgrind --leak-check=yes --track-origins=yes ./main

