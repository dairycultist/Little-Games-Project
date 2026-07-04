#ifndef WINDOW_H
#define WINDOW_H

#include <stdint.h>

// window size
#define WIDTH 256
#define HEIGHT 192

typedef struct {

	int up, 		up_justchanged;			// w
	int down, 		down_justchanged;		// a
	int left, 		left_justchanged;		// s
	int right, 		right_justchanged;		// d
	int action_a, 	action_a_justchanged;	// i
	int action_b, 	action_b_justchanged;	// j
	int action_c, 	action_c_justchanged;	// k
	int action_d, 	action_d_justchanged;	// l

} Input;

typedef struct {

	void *texture; // SDL_Texture *
	int w, h;

} Sprite;

// functions provided by the framework (implemented in window.c -- feel free to edit!) /////////////////

void load_sprite(Sprite *sprite_out, const char *path);
void unload_sprite(Sprite *sprite);

void draw_sprite(Sprite *sprite, int x, int y, int flip);
void draw_subsprite(Sprite *sprite, int w, int h, int from_x, int from_y, int to_x, int to_y, int flip);

void set_clear_color(uint8_t r, uint8_t g, uint8_t b);

void quit();

// YOU MUST IMPLEMENT THESE ////////////////////////////////////////////////////////////////////////////

const char *get_window_name();
void on_init();
void on_frame(const Input *input);
void on_quit();

#endif