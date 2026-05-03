#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <format>
//all programs data is here
static const int PIXELS = 24;
enum class GameState
{
	MENU,
	PLAYING,
	EXIT,
	PAUSE
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
struct SnakeState
{
	SDL_FRect headPos = { .x = 96,.y = 0, .w = 24,.h = 24 };
	std::vector<SDL_FRect> bodyPos;
	std::vector<SDL_FPoint> history;
	std::vector<SDL_FRect> foods;
	SnakeDirection currDir = SnakeDirection::RIGHT;
	float moveTimer = 0.0f;
	int count = 0;
	double angle;
	int score;
};
