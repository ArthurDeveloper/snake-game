#include <SDL2/SDL.h>
#include <list>
#include <iostream>
#include "directions.h"

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

	const int speed = 256;

public:
	int direction = RIGHT;

	Snake(SnakeNode first_node) {
		nodes.push_back(first_node);
	}

	void stick_to_screen() {
		SnakeNode& head = nodes.front();

		if (head.rect.x < 0) head.rect.x = 0;
		if (head.rect.x + head.rect.w > SCREEN_WIDTH) head.rect.x = SCREEN_WIDTH - head.rect.w;
		if (head.rect.y < 0) head.rect.y = 0;
		if (head.rect.y + head.rect.h > SCREEN_HEIGHT) head.rect.y = SCREEN_HEIGHT - head.rect.h;
	}

	void update(double delta) {
		for (auto& node : nodes) {
			if (direction == RIGHT) node.rect.x += speed * delta;
			else if (direction == LEFT) node.rect.x -= speed * delta;
			else if (direction == UP) node.rect.y -= speed * delta;
			else if (direction == DOWN) node.rect.y += speed * delta;

			node.update();
			stick_to_screen();
		}
	}

	void draw(SDL_Renderer* renderer) {
		for (auto& node : nodes) {
			node.draw(renderer);
		}
	}

	void change_direction(int new_direction) {
		direction = new_direction;
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
				if (event.key.keysym.sym == SDLK_RIGHT) snake.change_direction(RIGHT);
				else if (event.key.keysym.sym == SDLK_LEFT) snake.change_direction(LEFT);
				else if (event.key.keysym.sym == SDLK_UP) snake.change_direction(UP);
				else if (event.key.keysym.sym == SDLK_DOWN) snake.change_direction(DOWN);
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
