TARGET=image
CFLAGS=-Wall -Werror -lm -lSDL2 -lSDL2main -lSDL2_image

$(TARGET): sdl_hello.c
	gcc -o $@ $< $(CFLAGS)