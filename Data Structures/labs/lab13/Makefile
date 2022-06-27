CC = gcc
CFLAGS = -Wall -g

lista: test-graf.c graf-L.c
	$(CC) $(CFLAGS) $^ -o $@

matrice: test-graf.c graf-M.c
	$(CC) $(CFLAGS) $^ -o $@

run-lista:
	./lista

run-matrice:
	./matrice

.PHONY: clean
clean:
	rm -f *.o *~ lista
	rm -f *.o *~ matrice
