CC=gcc
CFLAGS=-Wall -g -std=gnu11
OBJ=tema2.o functiiLG.o functiiCoada.o functiiStiva.o
TARGET=tema2

build: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

run:
	./$(TARGET)

clean:
	rm -f *.o $(TARGET) *~

valgrind:
	valgrind --leak-check=yes --track-origins=yes ./$(TARGET)

