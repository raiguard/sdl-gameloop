all: sdl-demo

sdl-demo: *.c
	clang -lSDL2 -o sdl-demo main.c

run: sdl-demo
	./sdl-demo

.PHONY: run
