# Makefile

CC = gcc
CFLAGS = -Wall -Wextra -std=c99

TARGET = image_editor

build: $(TARGET)

image_editor: main.c command.c
	$(CC) $(CFLAGS) -o $(TARGET) main.c command.c

pack:
	zip -FSr 324CA_IleaDorin_Tema3.zip README.md Makefile main.c command.c structs.h

clean:
	rm -f $(TARGET)

.PHONY: pack clean
