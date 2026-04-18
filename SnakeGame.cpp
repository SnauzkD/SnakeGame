// SnakeGame.cpp : Defines the entry point for the application.
//

#include "SnakeGame.h"

int main(int arvc, char* argv[])
{
	SDLState state;
	Resourses rs;
	SnakeState sn;
	//
	if (!initiliaze(state,rs,sn))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), state.window);
		return 0;
	}
	bool running = true;
	uint64_t last = SDL_GetTicks();
	while (running)
	{
		SDL_Event event {0};
		uint64_t now = SDL_GetTicks();
		float deltatime = (now - last) / 1000.0f;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_EVENT_QUIT:
				{
					running = false;
					break;
				}
				case SDL_EVENT_KEY_DOWN:
				{
					if (event.key.scancode == SDL_SCANCODE_LEFT)
					{
						if (sn.dir != SnakeDirection::RIGHT)
						{
							sn.dir = SnakeDirection::LEFT;
						}
					}
					if (event.key.scancode == SDL_SCANCODE_RIGHT)
					{
						if (sn.dir != SnakeDirection::LEFT)
						{
							sn.dir = SnakeDirection::RIGHT;
						}
					}
					if (event.key.scancode == SDL_SCANCODE_DOWN)
					{
						if (sn.dir != SnakeDirection::UP)
						{
							sn.dir = SnakeDirection::DOWN;
						}
					}
					if (event.key.scancode == SDL_SCANCODE_UP)
					{
						if (sn.dir != SnakeDirection::DOWN)
						{
							sn.dir = SnakeDirection::UP;
						}
					}
				}
			}
		}
		SDL_SetRenderDrawColor(state.renderer,0,0,0,255);
		SDL_RenderClear(state.renderer);
		update(state,rs,sn,deltatime);
		SDL_RenderPresent(state.renderer);
		last = now;
	}
	
	cleanup(state,rs);
	return 0;
}


