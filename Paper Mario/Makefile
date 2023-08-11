TARGET=mario
IFLAG=-I
LFLAG=-L
CFLAGS=-Wall -Werror -Wno-unused
ifeq ($(OS),Windows_NT)
	TARGET:=$(TARGET).exe
	IFLAG += C:\SDL2_all\include
	LFLAG += C:\SDL2_all\lib
	CFLAGS += $(IFLAG) $(LFLAG) -lmingw32 -lm
else
	TARGET:=$(TARGET).out
	IFLAG += /mnt/c/SDL2_all/include
	LFLAG += /mnt/c/SDL2_all/lib
	CFLAGS += -lm
endif

#CFLAGS += $(IFLAG) $(LFLAG) -lm
SDLFLAGS=-lSDL2main -lSDL2 -lSDL2_image

$(TARGET): sdl_hello.c
	gcc -o $@ $< $(CFLAGS) $(SDLFLAGS)

.PHONY: clean run

run: $(TARGET)
	./$(TARGET)

clean:
	rm $(TARGET)