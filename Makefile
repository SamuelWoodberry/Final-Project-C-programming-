CC=gcc
CFLAGS=-Wall -Wextra -std=c99 -g
OBJ=main.o deck.o player.o game.o ui.o
TARGET=gofish

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f *.o $(TARGET)
