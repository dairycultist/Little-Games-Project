#include "window.h"

static Sprite test_sprite;

const char *get_window_name() {

	return "Demo";
}

void on_init() {

	load_sprite(&test_sprite, "test.png");
}

void on_frame(const Input *input) {

	draw_sprite(&test_sprite, WIDTH / 2 - 8, HEIGHT / 2 - 8, 0);
}

void on_quit() {}