// SnakeGame.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include "GameObject.h"
struct Resourses
{
	SDL_Texture* snakeHead, * snakeBody, * food, * background,*menuBackground
	,*startButton,*optionsButton,*exitButton,*snakeDead,*gameover,
	*backbutton,*optionBackground;
	std::vector<SDL_Texture*> textures;
	SDL_FRect backgroundRect, menuBackgroundRect, startButtonRect,exitButtonRect,optionButtonRect;
	MIX_Mixer* mixer;
	std::vector<MIX_Audio*> audios;
	MIX_Audio* eatFoodAudio, * backgroundMusic,*lofimusic;
	MIX_Track* musicTrack;

	VolumeSlider volume;
	DifficultyButton easyButton;
	DifficultyButton mediumButton;
	DifficultyButton hardButton;
	MIX_Audio* loadAudios(const std::string& filePath)
	{
		MIX_Audio* audio = MIX_LoadAudio(mixer, filePath.c_str(), false);
		audios.push_back(audio);
		return audio;
	}
	SDL_Texture* loadTexture(SDLState& state, Resourses& rs, const std::string& path)
	{
		SDL_Texture* tex;
		tex = IMG_LoadTexture(state.renderer, path.c_str());
		if (!tex)
		{
			std::cout << "Texture is NULL" << std::endl;
		}
		SDL_SetTextureScaleMode(tex, SDL_SCALEMODE_NEAREST);
		rs.textures.push_back(tex);
		return tex;
	}

	//initiliaze, create window and renderer
	bool load(SDLState& state, Resourses& rs, SnakeState& sn)
	{
		bool succes = true;
		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), state.window);
			succes = false;
			return succes;
		}
		if (!SDL_CreateWindowAndRenderer("SnakeGame", state.windowWidth,
			state.windowHeight, SDL_WINDOW_RESIZABLE, &state.window, &state.renderer))
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), state.window);
			succes = false;
			return succes;
		}
		if (!MIX_Init())
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to init MIX", nullptr);
			succes = false;
			return succes;
		}
		mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
		if (!mixer)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
			succes = false;
			return succes;
		}
		
		SDL_SetRenderLogicalPresentation(state.renderer, state.windowWidth, state.windowHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX);

		rs.backgroundRect = { .x = 0, .y = 0, .w = state.windowWidth,.h = state.windowHeight };
		rs.menuBackgroundRect = { .x = 0, .y = 0, .w = state.windowWidth,.h = state.windowHeight };
		rs.startButtonRect = { .x = state.windowWidth / 2  - 75 , .y = state.windowHeight / 2 - 75, .w = 150,.h = 50 };
		rs.optionButtonRect = { .x = state.windowWidth / 2 - 75 , .y = state.windowHeight / 2 + 5, .w = 150,.h = 50 };
		rs.exitButtonRect = { .x = state.windowWidth / 2 - 75 , .y = state.windowHeight / 2 + 75, .w = 150,.h = 50 };
		rs.snakeHead = loadTexture(state, rs, "snake_head.png");
		rs.snakeBody = loadTexture(state, rs, "snake_body.png");
		rs.snakeDead = loadTexture(state,rs,"snake_dead.png");
		rs.food = loadTexture(state, rs, "food.png");
		rs.background = loadTexture(state, rs, "background.png");
		rs.menuBackground = loadTexture(state,rs,"main_menu.png");
		rs.startButton = loadTexture(state,rs,"start_button.png");
		rs.exitButton = loadTexture(state,rs,"exit_button.png");
		rs.optionsButton = loadTexture(state,rs,"options_button.png");
		rs.gameover = loadTexture(state,rs,"gameover.png");
		rs.backbutton = loadTexture(state,rs,"back_button.png");
		rs.optionBackground = loadTexture(state,rs,"option_background.png");

		rs.eatFoodAudio = loadAudios("eating_apple.wav");
		rs.backgroundMusic = loadAudios("background_music_1.mp3");
		rs.lofimusic = loadAudios("background_music_3.mp3");
		//
		sn.score = 0;
		sn.bodyPos.clear();
		sn.history.push_back(SDL_FPoint{ .x = 48,.y = 0 });
		sn.history.push_back(SDL_FPoint{ .x = 72,.y = 0 });
		sn.history.push_back(SDL_FPoint{ .x = 96,.y = 0 });

		for (int i = 0; i < 3; i++)
		{
			sn.bodyPos.push_back(SDL_FRect(sn.history[i].x, sn.history[i].y, PIXELS, PIXELS));
		}
		volume.bar = { 30,75,100,20 };
		volume.handler = { volume.bar.x,volume.bar.y,20,20 };

		easyButton.button = {30,170,85,35};
		easyButton.text = "easy";

		mediumButton.button = {125,170,85,35};
		mediumButton.text = "medium";

		hardButton.button = {220,170,85,35};
		hardButton.text = "hard";

		return succes;

	}
	void unload(SDLState& state, Resourses& rs, SnakeState& sn)
	{
		for (auto* tex : rs.textures)
		{
			SDL_DestroyTexture(tex);
		}
		for (auto* audio : rs.audios)
		{
			MIX_DestroyAudio(audio);
		}
	}
};
void cleanup(SDLState& state, Resourses& rs)
{
	SDL_DestroyWindow(state.window);
	SDL_DestroyRenderer(state.renderer);
	MIX_DestroyTrack(rs.musicTrack);
	for (auto i : rs.textures)
	{
		SDL_DestroyTexture(i);
	}
}
//this function run every frame and update all objects on screen
void update(SDLState& state, Resourses& rs, SnakeState& sn,float deltatime)
{
	

	sn.moveTimer += deltatime;
	if(sn.moveTimer >= sn.moveDelay)
	{
		sn.moveTimer -= sn.moveDelay;
		sn.history.push_back(SDL_FPoint(sn.headPos.x,sn.headPos.y));
		switch (sn.currDir)
		{
		case SnakeDirection::LEFT:
		{
			sn.headPos.x -= PIXELS;
			break;
		}
		case SnakeDirection::RIGHT:
		{
			sn.headPos.x += PIXELS;
			break;
		}
		case SnakeDirection::DOWN:
		{
			sn.headPos.y += PIXELS;
			break;
		}
		case SnakeDirection::UP:
		{
			sn.headPos.y -= PIXELS;
			break;
		}
		}
		for (int i = 0; i < sn.foods.size(); i++)
		{
			if (sn.headPos.x == sn.foods[i].x
				&& sn.headPos.y == sn.foods[i].y)
			{
				MIX_PlayAudio(rs.mixer, rs.eatFoodAudio);
				sn.bodyPos.push_back(SDL_FRect(sn.history.back().x, sn.history.back().y, PIXELS, PIXELS));
				sn.foods.erase(sn.foods.begin() + i);
				sn.score++;
			}
		}
		for (int i = 0; i < sn.bodyPos.size(); i++)
		{
			if (sn.headPos.x == sn.bodyPos[i].x
				&& sn.headPos.y == sn.bodyPos[i].y)
			{
				currentState = GameState::DEAD;
			}
		}
		
	}
	if (sn.history.size() > 200)
	{
		sn.history.erase(sn.history.begin());
	}

	for (int i = 0; i < sn.bodyPos.size(); i++)
	{
		int idx = (int)sn.history.size() - 1 - i;
		if (idx >= 0)
		{
			sn.bodyPos[i].x = sn.history[idx].x;
			sn.bodyPos[i].y = sn.history[idx].y;
		}

	}


	//checking if the snake is on the screen
	if (sn.headPos.x >= state.windowWidth)
	{
		sn.headPos.x = 0;
	}
	else if (sn.headPos.x < 0)
	{
		sn.headPos.x = state.windowWidth - PIXELS;
	}
	if (sn.headPos.y >= state.windowHeight)
	{
		sn.headPos.y = 0;
	}
	else if (sn.headPos.y < 0)
	{
		sn.headPos.y = state.windowHeight - PIXELS;
	}

}

//this function run when program is end
void spawnFood(SDLState& state, Resourses& rs, SnakeState& sn)
{
	int attempts = 0;
	while(sn.foods.size()< 5 && attempts < 100)
	{
		attempts++;
		for (int i = sn.foods.size(); i < 5; i++)
		{
			bool canSpawn = true;
			int x = ((SDL_rand((state.windowWidth / 24))) * PIXELS);
			int y = ((SDL_rand((state.windowHeight / 24))) * PIXELS);
			for (int j = 0; j < sn.bodyPos.size(); j++)
			{
				if ((x == sn.bodyPos[j].x && y == sn.bodyPos[j].y) ||
					(x == sn.headPos.x && y == sn.headPos.y))
				{
					canSpawn = false;
					break;
				}
			}
			if (canSpawn)
			{
				sn.foods.push_back(SDL_FRect(x, y, PIXELS, PIXELS));
			}
			else
			{
				i--;
				continue;
			}
		}
	}
}
void reset(SDLState& state, Resourses& rs, SnakeState& sn)
{
	sn.score = 0;
	sn.bodyPos.clear();
	sn.foods.clear();
	sn.history.push_back(SDL_FPoint{ .x = 48,.y = 0 });
	sn.history.push_back(SDL_FPoint{ .x = 72,.y = 0 });
	sn.history.push_back(SDL_FPoint{ .x = 96,.y = 0 });

	rs.snakeHead = rs.snakeHead;
	sn.headPos = { .x = 96,.y = 0, .w = 24,.h = 24 };
	sn.currDir = SnakeDirection::RIGHT;
	sn.moveTimer = 0.0f;
	sn.score = 0;
	sn.dead = false;

	for (int i = 0; i < 3; i++)
	{
		sn.bodyPos.push_back(SDL_FRect(sn.history[i].x, sn.history[i].y, PIXELS, PIXELS));
	}
}
void saveGame(const SaveData& data)
{
	std::ofstream file("save.dat",std::ios::binary);

	if (file.is_open())
	{
		file.write(reinterpret_cast<const char*>(&data),sizeof(SaveData));
	}
}
bool loadGame(SaveData& data)
{
	std::ifstream file("save.dat",std::ios::binary);

	if(!file.is_open())
		return false;

	file.read(reinterpret_cast<char*>(&data),sizeof(SaveData));
	return true;
}
bool doesSaveExist()
{
	std::ifstream file("save.dat");
	return file.good();
}
// TODO: Reference additional headers your program requires here.
