.PHONY: run clean

build.out: *.c *.h
	gcc -o build.out *.c $(shell pkg-config --cflags --libs sdl2 SDL2_image)

run: build.out
	./build.out

clean:
	rm build.out