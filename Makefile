CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -I./include -D_POSIX_C_SOURCE=200112L
LDFLAGS = -lX11 -lpthread

SRC = src/main.c src/hybasic.c src/x11_backend.c src/text_mode.c \
      src/compositor.c src/input_handler.c src/config_parser.c
OBJ = $(SRC:.c=.o)
TARGET = hybasic

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

install: $(TARGET)
	cp $(TARGET) /usr/local/bin/
	mkdir -p /etc/hybasic
	cp config/hybasic.conf /etc/hybasic/
