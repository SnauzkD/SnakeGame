// SnakeGame.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include "GameObject.h"
//this structure contains all game resourses
struct Resources
{
	SDL_Texture* snakeHead, * snakeBody, * food, * first_level, * menuBackground,
		* startButton, * optionsButton, * exitButton, * snakeDead, * gameover,
		* backbutton, * optionBackground, * levelBackground, * second_level,
		* third_level, * fourth_level, * fiveth_level, * wall, * currLevelTexture;
	//
	//
	//
	std::vector<SDL_Texture*> textures;
	std::vector<MIX_Audio*> audios;
	std::vector<SDL_FRect> level2WallPos, level3WallPos, level4WallPos, level5WallPos;
	//
	SDL_FRect backgroundRect, menuBackgroundRect, startButtonRect, exitButtonRect, optionButtonRect;
	//
	//
	MIX_Mixer* mixer;
	MIX_Audio* eatFoodAudio, * backgroundMusic, * lofimusic;
	MIX_Track* musicTrack;
	//
	VolumeSlider volume;
	//
	DifficultyButton easyButton, mediumButton, hardButton;
	//
	Button level1, level2, level3, level4, level5;
	//
	LevelData levelData;
	//
	bool isThereWall = false;
	//load audio from filePath and push to the vector
	MIX_Audio* loadAudios(const std::string& filePath)
	{
		//load audio from filePath and push to vector
		MIX_Audio* audio = MIX_LoadAudio(mixer, filePath.c_str(), false);
		audios.push_back(audio);
		return audio;
	}
	//
	SDL_Texture* loadTexture(SDLState& state, Resources& rs, const std::string& path)
	{
		//load texture from filePath and check if the texture is not null and push to vector
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
	//
	bool load(SDLState& state, Resources& rs, SnakeState& sn)
	{
		//initiliaze window,renderer and mixer at the start of programm
		bool success = true;
		if (!SDL_Init(SDL_INIT_VIDEO))
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), state.window);
			success = false;
			return success;
		}
		//
		if (!SDL_CreateWindowAndRenderer("SnakeGame", state.windowWidth,
			state.windowHeight, SDL_WINDOW_RESIZABLE, &state.window, &state.renderer))
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), state.window);
			success = false;
			return success;
		}
		//
		if (!MIX_Init())
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Failed to init MIX", nullptr);
			success = false;
			return success;
		}
		//
		mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);
		if (!mixer)
		{
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
			success = false;
			return success;
		}
		//
		SDL_SetRenderLogicalPresentation(state.renderer, state.windowWidth, state.windowHeight, SDL_LOGICAL_PRESENTATION_LETTERBOX);

		//load and initiliaze all datas in programm
		rs.backgroundRect = { .x = 0, .y = 0, .w = state.windowWidth,.h = state.windowHeight };
		rs.menuBackgroundRect = { .x = 0, .y = 0, .w = state.windowWidth,.h = state.windowHeight };
		rs.startButtonRect = { .x = state.windowWidth / 2 - 75 , .y = state.windowHeight / 2 - 75, .w = 150,.h = 50 };
		rs.optionButtonRect = { .x = state.windowWidth / 2 - 75 , .y = state.windowHeight / 2 + 5, .w = 150,.h = 50 };
		rs.exitButtonRect = { .x = state.windowWidth / 2 - 75 , .y = state.windowHeight / 2 + 75, .w = 150,.h = 50 };
		//
		rs.snakeHead = loadTexture(state, rs, "snake_head.png");
		rs.snakeBody = loadTexture(state, rs, "snake_body.png");
		rs.snakeDead = loadTexture(state, rs, "snake_dead.png");
		rs.food = loadTexture(state, rs, "food.png");
		rs.first_level = loadTexture(state, rs, "background.png");
		rs.menuBackground = loadTexture(state, rs, "main_menu.png");
		rs.startButton = loadTexture(state, rs, "start_button.png");
		rs.exitButton = loadTexture(state, rs, "exit_button.png");
		rs.optionsButton = loadTexture(state, rs, "options_button.png");
		rs.gameover = loadTexture(state, rs, "gameover.png");
		rs.backbutton = loadTexture(state, rs, "back_button.png");
		rs.optionBackground = loadTexture(state, rs, "option_background.png");
		rs.second_level = loadTexture(state, rs, "second_level.png");
		rs.third_level = loadTexture(state, rs, "third_level.png");
		rs.fourth_level = loadTexture(state, rs, "fourth_level.png");
		rs.fiveth_level = loadTexture(state, rs, "fiveth_level.png");
		rs.wall = loadTexture(state, rs, "wall.png");
		//
		rs.eatFoodAudio = loadAudios("eating_apple.wav");
		rs.backgroundMusic = loadAudios("background_music_1.mp3");
		rs.lofimusic = loadAudios("background_music_3.mp3");
		//
		sn.score = 0;
		sn.bodyPos.clear();
		//
		sn.history.push_back(SDL_FPoint{ .x = 48,.y = 0 });
		sn.history.push_back(SDL_FPoint{ .x = 72,.y = 0 });
		sn.history.push_back(SDL_FPoint{ .x = 96,.y = 0 });
		//
		for (int i = 0; i < 3; i++)
		{
			sn.bodyPos.push_back(SDL_FRect(sn.history[i].x, sn.history[i].y, PIXELS, PIXELS));
		}
		//
		volume.bar = { 30,75,100,20 };
		volume.handler = { volume.bar.x,volume.bar.y,20,20 };
		//
		easyButton.button = { 30,170,85,35 };
		easyButton.text = "easy";

		mediumButton.button = { 125,170,85,35 };
		mediumButton.text = "medium";

		hardButton.button = { 220,170,85,35 };
		hardButton.text = "hard";
		//
		//
		level1.position = { 30,290,60,25 };
		level1.text = "level 1";

		level2.position = { 100,290,60,25 };
		level2.text = "level 2";

		level3.position = { 170,290,60,25 };
		level3.text = "level 3";

		level4.position = { 240,290,60,25 };
		level4.text = "level 4";

		level5.position = { 310,290,60,25 };
		level5.text = "level 5";
		//
		//
		currLevelTexture = first_level;
		//
		level2WallPos.push_back(SDL_FRect{ 3 * PIXELS,3 * PIXELS,24,24 });
		level2WallPos.push_back(SDL_FRect{ 2 * PIXELS,3 * PIXELS,24,24 });
		level2WallPos.push_back(SDL_FRect{ 2 * PIXELS,2 * PIXELS,24,24 });

		level2WallPos.push_back(SDL_FRect{ 8 * PIXELS,8 * PIXELS,24,24 });
		level2WallPos.push_back(SDL_FRect{ 9 * PIXELS,8 * PIXELS,24,24 });
		level2WallPos.push_back(SDL_FRect{ 10 * PIXELS,8 * PIXELS,24,24 });
		level2WallPos.push_back(SDL_FRect{ 11 * PIXELS,8 * PIXELS,24,24 });

		level2WallPos.push_back(SDL_FRect{ 16 * PIXELS,11 * PIXELS,24,24 });
		level2WallPos.push_back(SDL_FRect{ 16 * PIXELS,12 * PIXELS,24,24 });
		level2WallPos.push_back(SDL_FRect{ 16 * PIXELS,13 * PIXELS,24,24 });
		level2WallPos.push_back(SDL_FRect{ 16 * PIXELS,14 * PIXELS,24,24 });

		level3WallPos.push_back(SDL_FRect{ 6 * PIXELS,3 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 7 * PIXELS,3 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 8 * PIXELS,3 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 9 * PIXELS,3 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 10 * PIXELS,3 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 11 * PIXELS,3 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 12 * PIXELS,3 * PIXELS,24,24 });

		level3WallPos.push_back(SDL_FRect{ 6 * PIXELS,3 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 7 * PIXELS,3 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 8 * PIXELS,3 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 9 * PIXELS,3 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 10 * PIXELS,3 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 11 * PIXELS,3 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 12 * PIXELS,3 * PIXELS,24,24 });

		level3WallPos.push_back(SDL_FRect{ 9 * PIXELS,8 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 9 * PIXELS,9 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 9 * PIXELS,10 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 9 * PIXELS,11 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 9 * PIXELS,12 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 9 * PIXELS,13 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 9 * PIXELS,14 * PIXELS,24,24 });

		level3WallPos.push_back(SDL_FRect{ 16 * PIXELS,13 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 17 * PIXELS,13 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 16 * PIXELS,14 * PIXELS,24,24 });
		level3WallPos.push_back(SDL_FRect{ 17 * PIXELS,14 * PIXELS,24,24 });

		level4WallPos.push_back(SDL_FRect{ 3 * PIXELS,3 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 4 * PIXELS,3 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 5 * PIXELS,3 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 6 * PIXELS,3 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 7 * PIXELS,3 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 8 * PIXELS,3 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 9 * PIXELS,3 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 10 * PIXELS,3 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 11 * PIXELS,3 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 12 * PIXELS,3 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 13 * PIXELS,3 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 14 * PIXELS,3 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 15 * PIXELS,3 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 16 * PIXELS,3 * PIXELS,24,24 });

		level4WallPos.push_back(SDL_FRect{ 19 * PIXELS,4 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 19 * PIXELS,5 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 19 * PIXELS,6 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 19 * PIXELS,7 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 19 * PIXELS,8 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 19 * PIXELS,9 * PIXELS,24,24 });

		level4WallPos.push_back(SDL_FRect{ 10 * PIXELS,6 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 10 * PIXELS,7 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 10 * PIXELS,8 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 10 * PIXELS,9 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 10 * PIXELS,10 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 10 * PIXELS,11 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 10 * PIXELS,12 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 10 * PIXELS,13 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 10 * PIXELS,14 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 10 * PIXELS,15 * PIXELS,24,24 });

		level4WallPos.push_back(SDL_FRect{ 2 * PIXELS,12 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 3 * PIXELS,12 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 4 * PIXELS,12 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 5 * PIXELS,12 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 6 * PIXELS,12 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 7 * PIXELS,12 * PIXELS,24,24 });

		level4WallPos.push_back(SDL_FRect{ 17 * PIXELS,15 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 18 * PIXELS,15 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 19 * PIXELS,15 * PIXELS,24,24 });
		level4WallPos.push_back(SDL_FRect{ 20 * PIXELS,15 * PIXELS,24,24 });

		level5WallPos.push_back(SDL_FRect{ 4 * PIXELS,3 * PIXELS,24,24 });
		level5WallPos.push_back(SDL_FRect{ 4 * PIXELS,4 * PIXELS,24,24 });
		level5WallPos.push_back(SDL_FRect{ 4 * PIXELS,5 * PIXELS,24,24 });
		level5WallPos.push_back(SDL_FRect{ 4 * PIXELS,6 * PIXELS,24,24 });
		level5WallPos.push_back(SDL_FRect{ 4 * PIXELS,7 * PIXELS,24,24 });

		level5WallPos.push_back(SDL_FRect{ 8 * PIXELS,10 * PIXELS,24,24 });
		level5WallPos.push_back(SDL_FRect{ 9 * PIXELS,10 * PIXELS,24,24 });
		level5WallPos.push_back(SDL_FRect{ 10 * PIXELS,10 * PIXELS,24,24 });
		level5WallPos.push_back(SDL_FRect{ 11 * PIXELS,10 * PIXELS,24,24 });
		level5WallPos.push_back(SDL_FRect{ 12 * PIXELS,10 * PIXELS,24,24 });

		level5WallPos.push_back(SDL_FRect{ 18 * PIXELS,12 * PIXELS,24,24 });
		level5WallPos.push_back(SDL_FRect{ 18 * PIXELS,13 * PIXELS,24,24 });
		level5WallPos.push_back(SDL_FRect{ 18 * PIXELS,14 * PIXELS,24,24 });
		level5WallPos.push_back(SDL_FRect{ 18 * PIXELS,15 * PIXELS,24,24 });
		level5WallPos.push_back(SDL_FRect{ 18 * PIXELS,16 * PIXELS,24,24 });
		//
		levelData.levelState = LevelState::LEVEL1;

		return success;
	}
	//
	void unload(SDLState& state, Resources& rs, SnakeState& sn)
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
//
void renderWall(SDLState& state, Resources& rs, std::vector<SDL_FRect> walls)
{
	for (auto wall : walls)
	{
		SDL_RenderTexture(state.renderer, rs.wall, nullptr, &wall);
	}
}
//
void cleanup(SDLState& state, Resources& rs)
{
	SDL_DestroyWindow(state.window);
	SDL_DestroyRenderer(state.renderer);
	MIX_DestroyTrack(rs.musicTrack);
	for (auto i : rs.textures)
	{
		SDL_DestroyTexture(i);
	}
}
//
bool checkWallCollision(SnakeState& sn, const SDL_FRect& object, const std::vector<SDL_FRect>& walls)
{
	//checking if the snake head did not hit to the wall
	for (const auto& wall : walls)
	{
		if (sn.headPos.x == wall.x && sn.headPos.y == wall.y)
		{
			return true;
		}
	}
	return false;
}
//
void spawnFood(SDLState& state, Resources& rs, SnakeState& sn)
{
	for (int i = sn.foods.size(); i < 5; i++)
	{
		bool canSpawn = true;
		//create x and y to set random food position at screen
		int x = ((SDL_rand((state.windowWidth / 24))) * PIXELS);
		int y = ((SDL_rand((state.windowHeight / 24))) * PIXELS);
		//loop for verify the food is not spawning in the body of snake
		for (int j = 0; j < sn.bodyPos.size(); j++)
		{
			if ((x == sn.bodyPos[j].x && y == sn.bodyPos[j].y) ||
				(x == sn.headPos.x && y == sn.headPos.y))
			{
				canSpawn = false;
				break;
			}
		}
		//if the current level is not LEVEL1,then we can render wall
		if (rs.levelData.levelState != LevelState::LEVEL1)
		{
			for (const auto& wall : rs.levelData.walls)
			{
				if (x == wall.x && y == wall.y)
				{
					canSpawn = false;
					break;
				}
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
//
void reset(SDLState& state, Resources& rs, SnakeState& sn)
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
//
void saveGame(const SaveData& data)
{
	std::ofstream file("save.dat", std::ios::binary);

	if (file.is_open())
	{
		file.write(reinterpret_cast<const char*>(&data), sizeof(SaveData));
	}
}
//
bool loadGame(SaveData& data)
{
	std::ifstream file("save.dat", std::ios::binary);

	if (!file.is_open())
		return false;

	file.read(reinterpret_cast<char*>(&data), sizeof(SaveData));
	return true;
}
//
bool doesSaveExist()
{
	std::ifstream file("save.dat");
	return file.good();
}
//this function run every frame and update all objects on screen
void update(SDLState& state, Resources& rs, SnakeState& sn, float deltatime)
{
	sn.moveTimer += deltatime;
	//
	if (sn.moveTimer >= sn.moveDelay)
	{
		sn.moveTimer -= sn.moveDelay;
		sn.history.push_back(SDL_FPoint(sn.headPos.x, sn.headPos.y));
		//
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
		//
		for (int i = 0; i < sn.foods.size(); i++)
		{
			//if snake head hit the food then snake eat food and become bigger
			if (sn.headPos.x == sn.foods[i].x
				&& sn.headPos.y == sn.foods[i].y)
			{
				//playing eatFoodAudio every time when snake eat
				MIX_PlayAudio(rs.mixer, rs.eatFoodAudio);
				sn.bodyPos.push_back(SDL_FRect(sn.history.back().x, sn.history.back().y, PIXELS, PIXELS));
				sn.foods.erase(sn.foods.begin() + i);
				sn.score++;
			}
		}
		for (int i = 0; i < sn.bodyPos.size(); i++)
		{
			//verify that snake did not hit at itself
			if (sn.headPos.x == sn.bodyPos[i].x
				&& sn.headPos.y == sn.bodyPos[i].y)
			{
				currentState = GameState::DEAD;
				break;
			}
		}
		if (rs.levelData.levelState != LevelState::LEVEL1)
		{
			//if snake hit the wall, it die
			if (checkWallCollision(sn, sn.headPos, rs.levelData.walls))
			{
				currentState = GameState::DEAD;
			}
		}
	}
	//clean history vector if it larger than 200, so we can keep it clean
	if (sn.history.size() > 200)
	{
		sn.history.erase(sn.history.begin());
	}
	//
	for (int i = 0; i < sn.bodyPos.size(); i++)
	{
		//moving body at the previous head position
		int idx = (int)sn.history.size() - 1 - i;
		if (idx >= 0)
		{
			sn.bodyPos[i].x = sn.history[idx].x;
			sn.bodyPos[i].y = sn.history[idx].y;
		}
	}

	//checking snake is not out of screen
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