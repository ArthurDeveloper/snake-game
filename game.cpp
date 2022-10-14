#include <SDL2/SDL.h>
#include <list>
#include <iostream>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480


class SnakeNode {
public:
	SDL_Rect rect;

	SnakeNode(int x, int y) {
		rect.x = x;
		rect.y = y;
		rect.w = 20;
		rect.h = 20;
	}

	void update() {

	}

	void draw(SDL_Renderer* renderer) {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &rect);
	}
};

class Snake {
private:
	std::list<SnakeNode> nodes;

	const int speed = 150;

public:

	Snake(SnakeNode first_node) {
		nodes.push_back(first_node);
	}

	void update(double delta) {
		for (auto& node : nodes) {
			node.rect.x += speed * delta;
			node.rect.y += speed * delta;

			node.update();
		}
	}

	void draw(SDL_Renderer* renderer) {
		for (auto& node : nodes) {
			node.draw(renderer);
		}
	}
	
};

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0) {
		std::cout << "Something went wrong. Try again later.";	
		return -1;
	}

	std::cout << "All working fine!";

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	window = SDL_CreateWindow("Snake game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, 0, 0);


	SnakeNode first_node(0, 0);
	Snake snake(first_node);


	double delta = 0;
	Uint32 now = SDL_GetPerformanceCounter();
	Uint32 last = 0;
	
	bool quit = false;
	Uint32 t;
	SDL_Event event;
	bool has_restarted = false;

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
				
			}
		}


		snake.update(delta);
		snake.draw(renderer);


		SDL_RenderPresent(renderer);
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);


		t = SDL_GetTicks() - t;
		if (t < 1000 / 60) {
			SDL_Delay(1000 / 60 - t);
		}
	}

	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
