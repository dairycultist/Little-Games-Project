#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "window.h"

static SDL_Renderer *renderer;
static SDL_Texture *screen_buffer;

static uint8_t clear_r, clear_g, clear_b;

void set_clear_color(uint8_t r, uint8_t g, uint8_t b) {

	clear_r = r;
	clear_g = g;
	clear_b = b;
}

void draw_sprite(Sprite *sprite, int x, int y, int flip) {

	SDL_Rect copy_rect = { 0, 0, sprite->w, sprite->h };
	SDL_Rect paste_rect = { x, y, sprite->w, sprite->h };

	SDL_RenderCopyEx(renderer, (SDL_Texture *) sprite->texture, &copy_rect, &paste_rect, 0.0, NULL, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void draw_subsprite(Sprite *sprite, int w, int h, int from_x, int from_y, int to_x, int to_y, int flip) {

	SDL_Rect copy_rect = { from_x, from_y, w, h };
	SDL_Rect paste_rect = { to_x, to_y, w, h };

	SDL_RenderCopyEx(renderer, (SDL_Texture *) sprite->texture, &copy_rect, &paste_rect, 0.0, NULL, flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void load_sprite(Sprite *sprite_out, const char *path) {

	sprite_out->texture = IMG_LoadTexture(renderer, path);

	if (!sprite_out->texture) {
		fprintf(stderr, "Error loading sprite %s:\n%s\n", path, IMG_GetError());
		exit(1);
	}

	SDL_QueryTexture((SDL_Texture *) sprite_out->texture, NULL, NULL, &sprite_out->w, &sprite_out->h);
}

void unload_sprite(Sprite *sprite) {

	SDL_DestroyTexture((SDL_Texture *) sprite->texture);
	sprite->texture = NULL;
}

void quit() {

	on_quit();
	exit(0);
}

int main(void) {

	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		fprintf(stderr, "Error initializing SDL:\n%s\n", SDL_GetError());
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow(get_window_name(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH * 2, HEIGHT * 2, SDL_WINDOW_RESIZABLE);

	if (!window) {
		fprintf(stderr, "Error creating window:\n%s\n", SDL_GetError());
		return 1;
    }

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

	if (!renderer) {
		fprintf(stderr, "Error creating renderer:\n%s\n", SDL_GetError());
		return 1;
	}

	screen_buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);

	if (!screen_buffer) {
		fprintf(stderr, "Error creating screen buffer:\n%s\n", SDL_GetError());
		return 1;
	}

	on_init();

	// main loop
	SDL_Event event = { 0 };
	SDL_Rect letterbox = { 0, 0, WIDTH * 2, HEIGHT * 2 };
	Input input = { 0 };

	while (1) {

		input.up_justchanged       = 0;
		input.down_justchanged     = 0;
		input.left_justchanged     = 0;
		input.right_justchanged    = 0;
		input.action_a_justchanged = 0;
		input.action_b_justchanged = 0;
		input.action_c_justchanged = 0;
		input.action_d_justchanged = 0;
		
		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {

				quit();

			} else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED) {

				#define MIN(a, b) ((a) > (b) ? (b) : (a))
				#define ASPECT_RATIO (WIDTH / (float) HEIGHT)

				// dynamically change letterbox based on screen resize
				letterbox.w = MIN(event.window.data1, event.window.data2 * ASPECT_RATIO);
				letterbox.h = MIN(event.window.data2, event.window.data1 / ASPECT_RATIO);

				letterbox.x = (event.window.data1 - letterbox.w) / 2;
				letterbox.y = (event.window.data2 - letterbox.h) / 2;

			} else if ((event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) && !event.key.repeat) {

				switch (event.key.keysym.scancode) {
					case SDL_SCANCODE_W:
						input.up = event.key.state == SDL_PRESSED;
						input.up_justchanged = 1;
						break;
					case SDL_SCANCODE_S:
						input.down = event.key.state == SDL_PRESSED;
						input.down_justchanged = 1;
						break;
					case SDL_SCANCODE_A:
						input.left = event.key.state == SDL_PRESSED;
						input.left_justchanged = 1;
						break;
					case SDL_SCANCODE_D:
						input.right = event.key.state == SDL_PRESSED;
						input.right_justchanged = 1;
						break;
					case SDL_SCANCODE_I:
						input.action_a = event.key.state == SDL_PRESSED;
						input.action_a_justchanged = 1;
						break;
					case SDL_SCANCODE_J:
						input.action_b = event.key.state == SDL_PRESSED;
						input.action_b_justchanged = 1;
						break;
					case SDL_SCANCODE_K:
						input.action_c = event.key.state == SDL_PRESSED;
						input.action_c_justchanged = 1;
						break;
					case SDL_SCANCODE_L:
						input.action_d = event.key.state == SDL_PRESSED;
						input.action_d_justchanged = 1;
						break;
					default:
						break;
				}
			}
		}

		SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255); 					// clear window to grey
		SDL_RenderClear(renderer);
		SDL_SetRenderTarget(renderer, screen_buffer); 						// set render target to screen_buffer
		SDL_SetRenderDrawColor(renderer, clear_r, clear_g, clear_b, 255); 	// clear screen_buffer to clear color
		SDL_RenderClear(renderer);

		on_frame(&input);

		SDL_SetRenderTarget(renderer, NULL); 						// reset render target back to window
		SDL_RenderCopy(renderer, screen_buffer, NULL, &letterbox); 	// render screen_buffer
		SDL_RenderPresent(renderer); 								// present rendered content to screen

		SDL_Delay(1000 / 60);
	}
}