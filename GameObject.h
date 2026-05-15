#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <format>
#include <fstream>
//all programs data in GameObject.h
static const int PIXELS = 24;
enum class GameState
{
	MENU,
	PLAYING,
	EXIT,
	PAUSE,
	DEAD,
	OPTION,
	VIEW
};
GameState currentState = GameState::MENU;
enum class SnakeDirection
{
	LEFT,
	DOWN,
	RIGHT,
	UP
};
enum class DifficultMode
{
	EASY,
	MEDIUM,
	HARD
};
enum class LevelState
{
	LEVEL1,
	LEVEL2,
	LEVEL3,
	LEVEL4,
	LEVEL5
};
struct LevelData
{
	LevelState levelState;
	std::vector<SDL_FRect> walls;
};
struct SDLState
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	float windowWidth = 22 * PIXELS;
	float windowHeight = 18 * PIXELS;
};
struct SnakeState
{
	SDL_FRect headPos = { .x = 96,.y = 0, .w = 24,.h = 24 };
	std::vector<SDL_FRect> bodyPos;
	std::vector<SDL_FPoint> history;
	std::vector<SDL_FRect> foods;
	SnakeDirection currDir = SnakeDirection::RIGHT;
	DifficultMode mode = DifficultMode::MEDIUM;
	LevelState currLevel = LevelState::LEVEL1;
	float moveTimer = 0.0f;
	int score = 0;
	int bestscore = 0;
	bool dead = false;
	std::string difficulty = "medium";
	std::string level = "1";
	float moveDelay = 0.2f;
};

struct VolumeSlider
{
	SDL_FRect bar;
	SDL_FRect handler;

	float handlerPos;
	float value = 0.0f;

	bool dragging = false;
};
struct Button
{
	SDL_FRect position;
	std::string text;
};
struct DifficultyButton
{
	SDL_FRect button;
	std::string text;
};
struct SaveData
{
	float volume = 0;
	float handlerPos = 30;
};
