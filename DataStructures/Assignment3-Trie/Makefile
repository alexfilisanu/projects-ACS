CC=gcc
CFLAGS=-Wall -g -std=gnu11
OBJ=tema3.o trie.o
TARGET=tema3

build: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

run:
	./$(TARGET)

.PHONY: clean
clean:
	rm -f *.o $(TARGET) *~
