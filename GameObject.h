#pragma once
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <format>
//all programs data is here
struct SDLState
{
	SDL_Window* window;
	SDL_Renderer* renderer;
	int pixels = 24;
	int windowWidth = 22 * 24;
	int windowHeight = 18 * 24;
};
struct Resourses
{
	SDL_Texture* headTex;
	std::vector<SDL_Texture*> bodyTexs;
	std::vector<SDL_Texture*> textures;

};
struct SnakeState
{
	SDL_Rect headPos;
	std::vector<SDL_Rect> bodyPos;
	std::vector<SDL_Rect> history;
	int length = 3;
};