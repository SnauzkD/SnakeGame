// SnakeGame.cpp : Defines the entry point for the application.
//

#include "SnakeGame.h"
void updateGame(SDLState& state, Resourses& rs, SnakeState& sn, float deltatime);
void renderGame(SDLState& state, Resourses& rs, SnakeState& sn);
void renderMenu(SDLState& state, Resourses& rs, SnakeState& sn);
void updateMenu(SDLState& state, Resourses& rs, SnakeState& sn);
bool isPointInRect(int x, int y, SDL_FRect& rect);
int main(int arvc, char* argv[])
{
	SDLState state;
	Resourses rs;
	SnakeState sn;
	//
	if (!rs.load(state,rs,sn))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), state.window);
		return 0;
	}
	spawnFood(state, rs, sn);
	bool running = true;
	SDL_FRect drst = { state.windowWidth / 2 - 75,state.windowHeight / 2 - 35, 150,50 };
	uint64_t last = SDL_GetTicks();
	//Create a track to play it on
	MIX_Track* musicTrack = MIX_CreateTrack(rs.mixer);
	MIX_SetTrackAudio(musicTrack, rs.backgroundMusic);

	//Setup properties for looping
	SDL_PropertiesID props = SDL_CreateProperties();
	SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1); // -1 for infinite loop
	MIX_PlayTrack(musicTrack, props);
	// Clean up properties
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
					currentState = GameState::EXIT;
					break;
				}
				case SDL_EVENT_KEY_DOWN:
				{
					if (event.key.scancode == SDL_SCANCODE_ESCAPE)
					{
						if (currentState == GameState::PLAYING)
						{
							currentState = GameState::PAUSE;
						}
						else if (currentState == GameState::PAUSE)
						{
							currentState = GameState::PLAYING;
						}
					}
					if (currentState == GameState::PLAYING)
					{
						if (event.key.scancode == SDL_SCANCODE_LEFT)
						{
							if (sn.currDir != SnakeDirection::RIGHT)
							{
								sn.currDir = SnakeDirection::LEFT;
							}
						}
						if (event.key.scancode == SDL_SCANCODE_RIGHT)
						{
							if (sn.currDir != SnakeDirection::LEFT)
							{
								sn.currDir = SnakeDirection::RIGHT;
							}
						}
						if (event.key.scancode == SDL_SCANCODE_DOWN)
						{
							if (sn.currDir != SnakeDirection::UP)
							{
								sn.currDir = SnakeDirection::DOWN;
							}
						}
						if (event.key.scancode == SDL_SCANCODE_UP)
						{
							if (sn.currDir != SnakeDirection::DOWN)
							{
								sn.currDir = SnakeDirection::UP;
							}
						}
					}
					break;
				}
				case SDL_EVENT_MOUSE_BUTTON_DOWN:
				{
					int windowW, windowH;
					SDL_GetRenderOutputSize(state.renderer, &windowW, &windowH);
					float scaleX = windowW / 526.0f;
					float scaleY = windowH / 432.0f;
					int mouseX = event.button.x / scaleX;
					int mouseY = event.button.y / scaleY;
					if (currentState == GameState::MENU)
					{
						if (isPointInRect(mouseX, mouseY, rs.startButtonRect))
						{
							currentState = GameState::PLAYING;
						}
					}
					if (currentState == GameState::PAUSE)
					{
						if (isPointInRect(mouseX, mouseY, drst))
						{
							currentState = GameState::MENU;
						}
					}
					break;
				}
			}
		}
		if (currentState == GameState::MENU)
		{
			renderMenu(state,rs,sn);
			updateMenu(state,rs,sn);
		}
		else if (currentState == GameState::PLAYING)
		{
			MIX_ResumeTrack(musicTrack);
			updateGame(state,rs,sn,deltatime);
			renderGame(state, rs, sn);
		}
		else if (currentState == GameState::PAUSE)
		{
			MIX_PauseTrack(musicTrack);
			renderGame(state,rs,sn);
			SDL_SetRenderDrawBlendMode(state.renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 150);
			SDL_FRect overlay = { 0, 0, 528, 432 };
			SDL_RenderFillRect(state.renderer, &overlay);
			SDL_RenderTexture(state.renderer, rs.exitButton, nullptr, &drst);
		}

		SDL_RenderPresent(state.renderer);
		last = now;
	}
	SDL_DestroyProperties(props);
	rs.unload(state,rs,sn);
	cleanup(state,rs);
}
void updateGame(SDLState& state,Resourses& rs, SnakeState& sn,float deltatime)
{
	update(state, rs, sn, deltatime);
	spawnFood(state, rs, sn);
}
void renderGame(SDLState& state, Resourses& rs, SnakeState& sn)
{
	SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
	SDL_RenderClear(state.renderer);
	SDL_RenderTexture(state.renderer, rs.background, nullptr, &rs.backgroundRect);
	SDL_RenderTexture(state.renderer, rs.snakeHead, nullptr, &sn.headPos);
	for (int i = 0; i < sn.bodyPos.size(); i++)
	{
		SDL_RenderTexture(state.renderer, rs.snakeBody, nullptr, &sn.bodyPos[i]);
	}
	for (int i = 0; i < 5; i++)
	{
		SDL_RenderTexture(state.renderer, rs.food, nullptr, &sn.foods[i]);
	}
	SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
	SDL_SetRenderScale(state.renderer, 1.5, 1.5);
	SDL_RenderDebugText(state.renderer, 5, 5, std::format("score:{}", sn.score).c_str());
	SDL_SetRenderScale(state.renderer, 1.0, 1.0);

}
void renderMenu(SDLState& state, Resourses& rs, SnakeState& sn)
{
	SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
	SDL_RenderClear(state.renderer);
	SDL_RenderTexture(state.renderer, rs.menuBackground, nullptr, &rs.menuBackgroundRect);
	SDL_RenderTexture(state.renderer,rs.startButton,nullptr,&rs.startButtonRect);
	SDL_RenderTexture(state.renderer, rs.exitButton, nullptr, &rs.exitButtonRect);
	SDL_RenderTexture(state.renderer, rs.optionsButton, nullptr, &rs.optionButtonRect);
}
void updateMenu(SDLState& state, Resourses& rs, SnakeState& sn)
{
	
}
bool isPointInRect(int x, int y, SDL_FRect& rect)
{
	return (x >= rect.x &&
		x <= rect.x + rect.w &&
		y >= rect.y &&
		y <= rect.y + rect.h);
}

