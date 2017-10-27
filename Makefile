.PHONY: debug
all: debug
debug:
	gcc main.c `pkg-config --cflags --libs allegro_monolith-5` -o RPG
	./RPG