# makefile for MinGW64/MSYS2 on Windows

CFLAGS := -Wall -O2 #-fsanitize=address,undefined
CC := gcc
LDFLAGS := #-fsanitize=address,undefined

SRC = $(wildcard ./src/*.c)

OBJ := $(patsubst %.c,%.o,$(filter %.c,$(SRC)))

TARGET = main.exe

all: $(TARGET)
#	 ./$(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: clean all
