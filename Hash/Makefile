CC = gcc
CFLAGS = -Wall -g -Iinclude -Iunity

SRC = hash_extensivel.c
TEST = test_hash.c
UNITY = unity.c

TARGET = test

all: $(TARGET)

$(TARGET):
	$(CC) $(CFLAGS) $(SRC) $(TEST) $(UNITY) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full --track-origins=yes ./$(TARGET)

clean:
	rm -f $(TARGET)