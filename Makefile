.PHONY: debug
all: debug
debug:
	gcc -Wall algif5/algif.c algif5/bitmap.c algif5/gif.c algif5/lzw.c main.c `pkg-config --cflags --libs allegro_monolith-5` -lm -o debug/main
	debug/main