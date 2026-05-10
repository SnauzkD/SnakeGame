// SnakeGame.cpp : Defines the entry point for the application.
//

#include "SnakeGame.h"
void updateGame(SDLState& state, Resourses& rs, SnakeState& sn, float deltatime);
void renderGame(SDLState& state, Resourses& rs, SnakeState& sn);
void renderMenu(SDLState& state, Resourses& rs, SnakeState& sn);
void updateMenu(SDLState& state, Resourses& rs, SnakeState& sn);
void renderOption(SDLState& state, Resourses& rs, SnakeState& sn);
void renderSlider(SDLState& state, Resourses& rs);
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
	SDL_FRect dstrectButton = { .x = state.windowWidth / 2 - 75,.y = 250,.w = 150 ,.h = 50 };
	SDL_FRect dsrectBack = { .x = 10,.y = 350,.w = 150,.h = 50 };
	uint64_t last = SDL_GetTicks();
	//Create a track to play it on
	rs.musicTrack = MIX_CreateTrack(rs.mixer);
	MIX_SetTrackAudio(rs.musicTrack, rs.lofimusic);
	MIX_SetTrackGain(rs.musicTrack,0.0f);

	//Setup properties for looping
	SDL_PropertiesID props = SDL_CreateProperties();
	SDL_SetNumberProperty(props, MIX_PROP_PLAY_LOOPS_NUMBER, -1); // -1 for infinite loop
	MIX_PlayTrack(rs.musicTrack, props);

	if (doesSaveExist())
	{
		SaveData data;
		if (loadGame(data))
		{
			rs.volume.value = data.volume;
			sn.bestscore = data.highscore;
		}
	}
	// Clean up properties
	while (running)
	{
		SDL_Event event {0};
		uint64_t now = SDL_GetTicks();
		float deltatime = (now - last) / 1000.0f;
		while (SDL_PollEvent(&event))
		{
			SDL_ConvertEventToRenderCoordinates(state.renderer,&event);
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
					int mouseX = event.button.x;
					int mouseY = event.button.y;
					if (currentState == GameState::MENU)
					{
						if (isPointInRect(mouseX, mouseY, rs.startButtonRect))
						{
							MIX_SetTrackAudio(rs.musicTrack, rs.backgroundMusic);
							MIX_ResumeTrack(rs.musicTrack);
							currentState = GameState::PLAYING;
						}
						if (isPointInRect(mouseX, mouseY, rs.exitButtonRect))
						{
							currentState = GameState::EXIT;
						}
						if (isPointInRect(mouseX, mouseY, rs.optionButtonRect))
						{
							currentState = GameState::OPTION;
						}
					}
					if (currentState == GameState::PAUSE)
					{
						if (isPointInRect(mouseX, mouseY, drst))
						{
							MIX_SetTrackAudio(rs.musicTrack, rs.lofimusic);
							MIX_ResumeTrack(rs.musicTrack);
							currentState = GameState::MENU;
						}
					}
					if (currentState == GameState::DEAD)
					{
						if (isPointInRect(mouseX, mouseY, dstrectButton))
						{
							MIX_SetTrackAudio(rs.musicTrack, rs.lofimusic);
							MIX_ResumeTrack(rs.musicTrack);
							currentState = GameState::MENU;
							reset(state,rs,sn);
						}
					}
					if (currentState == GameState::OPTION)
					{
						if (isPointInRect(mouseX, mouseY, dsrectBack))
						{
							currentState = GameState::MENU;
						}
						SDL_FRect grabArea =
						{
							rs.volume.handler.x - 10,
							rs.volume.handler.y - 10,
							rs.volume.handler.w + 20,
							rs.volume.handler.h + 20
						};
						if (isPointInRect(mouseX, mouseY, grabArea))
						{
							rs.volume.dragging = true;
						}
						if (isPointInRect(mouseX, mouseY, rs.easyButton.button))
						{
							sn.mode = DifficultMode::EASY;
						}
						if (isPointInRect(mouseX, mouseY, rs.mediumButton.button))
						{
							sn.mode = DifficultMode::MEDIUM;
						}
						if (isPointInRect(mouseX, mouseY, rs.hardButton.button))
						{
							sn.mode = DifficultMode::HARD;
						}
					}
					break;
				}
				case SDL_EVENT_MOUSE_BUTTON_UP:
				{
					if(currentState == GameState::OPTION)
					{
						rs.volume.dragging = false;
					}
					break;
				}
				case SDL_EVENT_MOUSE_MOTION:
				{
		
					if (currentState == GameState::OPTION)
					{
						if (rs.volume.dragging)
						{
							float mx = event.motion.x;

							if(mx < rs.volume.bar.x)
								mx = rs.volume.bar.x;

							if(mx > rs.volume.bar.x + rs.volume.bar.w)
								mx = rs.volume.bar.x + rs.volume.bar.w;

							rs.volume.handler.x = mx - rs.volume.handler.w / 2;

							rs.volume.value = (mx - rs.volume.bar.x) / rs.volume.bar.w;

							MIX_SetMixerGain(rs.mixer, rs.volume.value);
							MIX_SetTrackGain(rs.musicTrack,rs.volume.value);
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
			updateGame(state,rs,sn,deltatime);
			renderGame(state, rs, sn);
		}
		else if (currentState == GameState::PAUSE)
		{
			MIX_PauseTrack(rs.musicTrack);
			renderGame(state,rs,sn);
			SDL_SetRenderDrawBlendMode(state.renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 150);
			SDL_FRect overlay = { 0, 0, state.windowWidth, state.windowHeight };
			SDL_RenderFillRect(state.renderer, &overlay);
			SDL_RenderTexture(state.renderer, rs.backbutton, nullptr, &drst);
		}
		else if (currentState == GameState::EXIT)
		{
			running = false;
		}
		else if (currentState == GameState::DEAD)
		{
			SDL_FRect dstrect = { .x = state.windowWidth / 2 - 130,.y = 100,.w = 250 ,.h = 75 };
			MIX_PauseTrack(rs.musicTrack);
			renderGame(state,rs,sn);
			SDL_SetRenderDrawBlendMode(state.renderer, SDL_BLENDMODE_BLEND);
			SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 150);
			SDL_FRect overlay = { 0, 0, state.windowWidth, state.windowHeight };
			SDL_RenderFillRect(state.renderer, &overlay);
			SDL_RenderTexture(state.renderer,rs.gameover,nullptr,&dstrect);
			SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
			SDL_RenderTexture(state.renderer, rs.backbutton, nullptr, &dstrectButton);
			float scale = 2.0f;
			SDL_SetRenderScale(state.renderer, scale, scale);
			SDL_RenderDebugText(state.renderer, (state.windowWidth / 2 - 50) / scale, 200 / scale, std::format("score:{}", sn.score).c_str());
			SDL_SetRenderScale(state.renderer, 1.0, 1.0);
			if (sn.score > sn.bestscore)
			{
				sn.bestscore = sn.score;
			}
		}
		else if (currentState == GameState::OPTION)
		{
			renderOption(state,rs,sn);
			renderSlider(state,rs);
			float scale = 2.0f;
			SDL_SetRenderScale(state.renderer, scale, scale);
			SDL_RenderDebugText(state.renderer, (rs.volume.bar.x) / scale,
			(rs.volume.bar.y - 35) / scale, std::format("music Volume:{}", rs.volume.value).c_str());
			SDL_RenderDebugText(state.renderer, (rs.volume.bar.x) / scale,
				(rs.volume.bar.y + 50) / scale, std::format("difficulty:{}", sn.difficulty).c_str());
			SDL_RenderDebugText(state.renderer, (rs.volume.bar.x) / scale,
				(rs.volume.bar.y + 170) / scale, std::format("best score:{}", sn.bestscore).c_str());
			SDL_SetRenderScale(state.renderer, 1.0, 1.0);
		}
		if (sn.mode == DifficultMode::EASY)
		{
			sn.difficulty = "easy";
			sn.moveDelay = 0.15f;
		}
		else if (sn.mode == DifficultMode::MEDIUM)
		{
			sn.difficulty = "medium";
			sn.moveDelay = 0.1f;
		}
		else if (sn.mode == DifficultMode::HARD)
		{
			sn.difficulty = "hard";
			sn.moveDelay = 0.05f;
		}

		SDL_RenderPresent(state.renderer);
		last = now;
	}
	SaveData data;
	data.highscore = sn.bestscore;
	data.volume = rs.volume.value;
	saveGame(data);
	SDL_DestroyProperties(props);
	rs.unload(state,rs,sn);
	cleanup(state,rs);
}
void updateGame(SDLState& state,Resourses& rs, SnakeState& sn,float deltatime)
{
	update(state, rs, sn, deltatime);
	spawnFood(state, rs, sn);
}
void renderSlider(SDLState& state, Resourses& rs)
{
	SDL_SetRenderDrawColor(state.renderer, 120, 120, 120, 255);
	SDL_RenderFillRect(state.renderer, &rs.volume.bar);

	// handle
	SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(state.renderer, &rs.volume.handler);
}
void renderGame(SDLState& state, Resourses& rs, SnakeState& sn)
{
	SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
	SDL_RenderClear(state.renderer);
	SDL_RenderTexture(state.renderer, rs.background, nullptr, &rs.backgroundRect);
	for (int i = 0; i < sn.bodyPos.size(); i++)
	{
		SDL_RenderTexture(state.renderer, rs.snakeBody, nullptr, &sn.bodyPos[i]);
	}
	if (currentState == GameState::DEAD)
	{
		SDL_RenderTexture(state.renderer, rs.snakeDead, nullptr, &sn.headPos);
	}
	else
	{
		SDL_RenderTexture(state.renderer, rs.snakeHead, nullptr, &sn.headPos);
	}
	for (int i = 0; i < sn.foods.size(); i++)
	{
		SDL_RenderTexture(state.renderer, rs.food, nullptr, &sn.foods[i]);
	}
	float scale = 1.5f;
	SDL_SetRenderDrawColor(state.renderer, 255, 255, 255, 255);
	SDL_SetRenderScale(state.renderer, scale, scale);
	SDL_RenderDebugText(state.renderer, 5 / scale, 5 / scale, std::format("score:{}", sn.score).c_str());
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
void renderOption(SDLState& state, Resourses& rs, SnakeState& sn)
{	
	SDL_FRect dsrectBack = { .x = 20,.y = 350,.w = 150,.h = 50 };
	SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
	SDL_RenderClear(state.renderer);
	SDL_RenderTexture(state.renderer, rs.optionBackground, nullptr, &rs.menuBackgroundRect);
	SDL_RenderTexture(state.renderer, rs.backbutton, nullptr, &dsrectBack);

	float scale = 1.7f;
	SDL_SetRenderDrawColor(state.renderer, 120, 120, 120, 255);
	SDL_RenderFillRect(state.renderer,&rs.easyButton.button);
	SDL_RenderFillRect(state.renderer, &rs.mediumButton.button);
	SDL_RenderFillRect(state.renderer, &rs.hardButton.button);

	SDL_SetRenderDrawColor(state.renderer, 0, 0, 0, 255);
	SDL_SetRenderScale(state.renderer, scale, scale);
	SDL_RenderDebugText(state.renderer,(rs.easyButton.button.x + 15) / scale,(rs.easyButton.button.y + 10) / scale,rs.easyButton.text.c_str());
	SDL_RenderDebugText(state.renderer, (rs.mediumButton.button.x + 2) / scale, (rs.mediumButton.button.y + 10) / scale, rs.mediumButton.text.c_str());
	SDL_RenderDebugText(state.renderer, (rs.hardButton.button.x + 15) / scale, (rs.hardButton.button.y + 10) / scale, rs.hardButton.text.c_str());

	SDL_SetRenderScale(state.renderer, 1.0, 1.0);
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

