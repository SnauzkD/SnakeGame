#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <format>
//all programs data is here
#define PIXELS 24;
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
	int windowWidth = 22 * PIXELS;
	int windowHeight = 18 * PIXELS;
};
struct Resourses
{
	SDL_Texture* headRightTex,* headLeftTex,* headDownTex,* headUpTex;
	std::vector<SDL_Texture*> bodyTexs;
	std::vector<SDL_Texture*> textures;

};
struct SnakeState
{
	SDL_FRect headPos = {.x = 0,.y = 0, .w = 24,.h = 24};
	std::vector<SDL_FRect> bodyPos;
	std::vector<SDL_FRect> history;
	SDL_Texture* snakeTexture;
	int length = 3;
	SnakeDirection dir = SnakeDirection::RIGHT;
	float moveTimer = 0.0f;
};