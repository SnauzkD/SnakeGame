#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <format>
#include <fstream>
//all programs data is here
static const int PIXELS = 24;
enum class GameState
{
	MENU,
	PLAYING,
	EXIT,
	PAUSE,
	DEAD,
	OPTION
};
GameState currentState = GameState::MENU;
enum class SnakeDirection
{
	LEFT,
	DOWN,
	RIGHT,
	UP
};
struct SDLState
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	float windowWidth = 22 * PIXELS;
	float windowHeight = 18 * PIXELS;
};
enum class DifficultMode
{
	EASY,
	MEDIUM,
	HARD
};
struct SnakeState
{
	SDL_FRect headPos = { .x = 96,.y = 0, .w = 24,.h = 24 };
	std::vector<SDL_FRect> bodyPos;
	std::vector<SDL_FPoint> history;
	std::vector<SDL_FRect> foods;
	SnakeDirection currDir = SnakeDirection::RIGHT;
	DifficultMode mode = DifficultMode::MEDIUM;
	float moveTimer = 0.0f;
	int score = 0;
	int bestscore = 0;
	bool dead = false;
	std::string difficulty = "medium";
	float moveDelay = 0.2f;
};

struct VolumeSlider
{
	SDL_FRect bar;
	SDL_FRect handler;

	float value = 0.0f;

	bool dragging = false;
};
struct DifficultyButton
{
	SDL_FRect button;
	std::string text;
};
struct SaveData
{
	int highscore = 0;
	float volume = 0;
};
