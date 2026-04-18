// SnakeGame.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include "GameObject.h"

void update(SDLState& state, Resourses& rs, SnakeState& sn,float deltatime)
{
	float moveDelay = 0.1f;
	sn.moveTimer += deltatime;
	if (sn.moveTimer >= moveDelay)
	{
		sn.moveTimer -= moveDelay;
		switch (sn.dir)
		{
		case SnakeDirection::LEFT:
		{
			sn.headPos.x -= PIXELS;
			sn.snakeTexture = rs.headLeftTex;
			break;
		}
		case SnakeDirection::RIGHT:
		{
			sn.headPos.x += PIXELS;
			sn.snakeTexture = rs.headRightTex;
			break;
		}
		case SnakeDirection::DOWN:
		{
			sn.headPos.y += PIXELS;
			sn.snakeTexture = rs.headDownTex;
			break;
		}
		case SnakeDirection::UP:
		{
			sn.headPos.y -= PIXELS;
			sn.snakeTexture = rs.headUpTex;
			break;
		}
		}
	}
	SDL_RenderTexture(state.renderer, sn.snakeTexture, nullptr, &sn.headPos);
	//SDL_SetRenderDrawColor(state.renderer,255,0,0,255);
	//SDL_RenderFillRect(state.renderer,&sn.headPos);
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
};
//initiliaze, create window and renderer
bool initiliaze(SDLState& state,Resourses& rs,SnakeState& sn)
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

	rs.headRightTex = loadTexture(state,rs,"head_right.png");
	rs.headLeftTex = loadTexture(state,rs,"head_left.png");
	rs.headDownTex = loadTexture(state, rs, "head_down.png");
	rs.headUpTex = loadTexture(state, rs, "head_up.png");
	//


	rs.headRightTex->w = PIXELS;
	rs.headRightTex->h = PIXELS;
	sn.snakeTexture = rs.headRightTex;

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
