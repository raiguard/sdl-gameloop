all:
	meson compile -C build

setup:
	meson setup --reconfigure build

run: all
	build/game

.PHONY: setup all run
