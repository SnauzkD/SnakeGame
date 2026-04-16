// SnakeGame.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include "GameObject.h"


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
};
//initiliaze, create window and renderer
bool initiliaze(SDLState& state,Resourses& rs)
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
	SDL_SetRenderLogicalPresentation(state.renderer,state.windowWidth,state.windowHeight,SDL_LOGICAL_PRESENTATION_LETTERBOX);

	rs.headTex = loadTexture(state,rs,"head_right.png");
	if (!rs.headTex)
	{
		return false;
	}
	return succes;

};
//this function run when program is end
void cleanup(SDLState& state,Resourses& rs)
{
	SDL_DestroyWindow(state.window);
	SDL_DestroyRenderer(state.renderer);

	for (auto i : rs.textures)
	{
		SDL_DestroyTexture(i);
	}
}
// TODO: Reference additional headers your program requires here.
