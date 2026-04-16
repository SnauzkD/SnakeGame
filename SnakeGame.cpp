// SnakeGame.cpp : Defines the entry point for the application.
//

#include "SnakeGame.h"

int main(int arvc, char* argv[])
{
	SDLState state;
	Resourses rs;
	SnakeState sn;
	//
	if (!initiliaze(state,rs))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), state.window);
		return 0;
	}
	SDL_FRect dst = { .x = 30.0, .y = 30.0,.w = ((float)state.pixels),.h = ((float)state.pixels) };
	bool running = true;
	while (running)
	{
		SDL_Event event {0};
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
				case SDL_EVENT_QUIT:
				{
					running = false;
					break;
				}
			}
		}
		SDL_SetRenderDrawColor(state.renderer,0,0,0,255);
		SDL_RenderClear(state.renderer);
		SDL_RenderTexture(state.renderer,rs.headTex,nullptr,&dst);
		SDL_RenderPresent(state.renderer);
	}
	
	cleanup(state,rs);
	return 0;
}


