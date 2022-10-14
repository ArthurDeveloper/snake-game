#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "Bee.h"
#include "Pipe.h"
#include <list>
#include <iostream>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


int amount_of_pipes = 0;
void addPipes(std::list<Pipe>& pipes, int amount_to_be_added) {
	amount_of_pipes += amount_to_be_added;
	
	for (int i = amount_of_pipes - amount_to_be_added; i < amount_of_pipes; i+=2) {
		int random_number = rand() % 320;

		Pipe pipe1(
			(SCREEN_WIDTH + 80 * (2 * i)),
			(300 + (rand() % 120)), 
			80,
			SCREEN_HEIGHT
		);

		Pipe pipe2(
			(SCREEN_WIDTH + 80 * (2 * i)),
			(300 + (rand() % 120)) * -1, 
			80,
			SCREEN_HEIGHT
		); 

		pipes.push_back(pipe1);
		pipes.push_back(pipe2);
	}	
}

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		std::cout << "Something went wrong. Try again later.";	
		return -1;
	}

	if (IMG_Init(IMG_INIT_PNG) == 0) {
		std::cout << "Something went wrong. Try again later.";
		return -1;
	}

	std::cout << "All working fine!";

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	window = SDL_CreateWindow("Flappy bee", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, 0, 0);


	Bee bee(50, SCREEN_HEIGHT/2-60, 64, 64);

	std::list<Pipe> pipes;
	addPipes(pipes, 20);

	float pipe_spawn_timer = 0;

	double delta = 0;
	Uint32 now = SDL_GetPerformanceCounter();
	Uint32 last = 0;
	
	bool quit = false;
	Uint32 t;
	SDL_Event event;
	bool has_restarted = false;

	SDL_Surface* you_died_text_surface = nullptr;
	SDL_Texture* you_died_text_texture = nullptr;
	while (!quit) {
		t = SDL_GetTicks();

		
		last = now;
		now = SDL_GetPerformanceCounter();

		delta = (double)((now - last) * 1000 / (double) SDL_GetPerformanceFrequency()) / 1000;

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}

			if (event.type == SDL_KEYDOWN) {
				if (
					event.key.keysym.sym == SDLK_UP || 
					event.key.keysym.sym == SDLK_SPACE ||
					event.key.keysym.sym == SDLK_w
				) {
					bee.jump();
				}

				if (event.key.keysym.sym == SDLK_RETURN && bee.has_died) {
					has_restarted = true;
				}
			}
		}
		
		if (!bee.has_died) {
			bee.update(delta);
		}
		bee.draw(renderer);
		
		if (!bee.is_between_boundaries(0, SCREEN_HEIGHT)) {
			bee.die();
		}

		bool pop_front = false;
		for (Pipe& pipe : pipes) {
			if (!bee.has_died) {
				pipe.update(delta);
			}
			pipe.draw(renderer);

			if (pipe.rect.x + pipe.rect.w < 0) {
				pop_front = true;
			}
						
			if (SDL_HasIntersection(&bee.rect, &pipe.rect)) {
				bee.die();
			}
		}

		if (pop_front) {
			amount_of_pipes--;
			pipes.pop_front();
		}
		
		pipe_spawn_timer += delta;
		if (pipe_spawn_timer > 1 && pipes.size() < 100) {
			addPipes(pipes, 20);
			pipe_spawn_timer = 0;
		}


		if (bee.has_died) {
			if (you_died_text_surface == nullptr && you_died_text_texture == nullptr) {
				you_died_text_surface = IMG_Load("res/you-died-text.png");
				if (you_died_text_surface == NULL) {
					std::cout << "Surface couldn't be loaded.";
					return -1;
				}

				you_died_text_texture = SDL_CreateTextureFromSurface(renderer, you_died_text_surface);
	
				SDL_FreeSurface(you_died_text_surface);
				
				you_died_text_surface = nullptr;
			}

			int width = 616;
			int height = 192;
			SDL_Rect rect = { SCREEN_WIDTH/2-width/2, SCREEN_HEIGHT/2-height/2, width, height };
			SDL_RenderCopy(renderer, you_died_text_texture, NULL, &rect);

			if (has_restarted) {
				SDL_DestroyTexture(you_died_text_texture);
				you_died_text_texture = nullptr;

				bee.has_died = false;
				bee.rect.x = bee.initial_x;
				bee.rect.y = bee.initial_y;
				bee.y_velocity = 0;

				pipes = {};
				amount_of_pipes = 0;
				addPipes(pipes, 80);
				
				has_restarted = false;
			}
		}


		SDL_RenderPresent(renderer);
		
		SDL_SetRenderDrawColor(renderer, 0, 220, 255, 255);
		SDL_RenderClear(renderer);


		t = SDL_GetTicks() - t;
		if (t < 1000 / 60) {
			SDL_Delay(1000 / 60 - t);
		}
	}

	SDL_DestroyWindow(window);
	SDL_DestroyTexture(bee.texture);
	SDL_Quit();
	IMG_Quit();

	return 0;
}
