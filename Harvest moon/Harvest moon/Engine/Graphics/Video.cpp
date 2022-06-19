#include "Video.h"
#include <iostream>

Video* Video::_videoInstance = NULL;


void Video::renderGraphic(int idTextura, Xywh* src, Xywh* dst)
{
	// Assignar RECTs SRC/DEST
	SDL_Rect srcRect;
	SDL_Rect destRect;

	srcRect.x = src->x;
	srcRect.y = src->y;
	srcRect.w = src->w;
	srcRect.h = src->h;

	destRect.x = dst->x;
	destRect.y = dst->y;
	destRect.w = dst->w;
	destRect.h = dst->h;

	SDL_RenderCopy(_renderer, _rscManager->getGraphicByID(idTextura), &srcRect, &destRect);
}

void Video::clearScreen()
{
	SDL_RenderClear(_renderer);
}

void Video::pintarColorScreen(Uint32 color)
{
	SDL_FillRect(_surface, NULL, color);
	SDL_UpdateWindowSurface(_window);
}

void Video::updateScreen()
{
	SDL_RenderPresent(_renderer);
}

void Video::waitTime(int ms)
{
	SDL_Delay(ms);
}

void Video::close()
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	SDL_Quit();
}


Video::~Video() {
	close();
}


Video::Video()
{
	_window = NULL;
	_surface = NULL;
	_renderer = NULL;
	_rscManager = NULL;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";

	//_window = SDL_CreateWindow("Harvest moon", 2550, 350, TAMANY_WORLD_W, TAMANY_WORLD_H, SDL_WINDOW_SHOWN);
	//_window = SDL_CreateWindow("Harvest moon", 1550, 50, CAMERA_W, CAMERA_H, SDL_WINDOW_SHOWN);
	_window = SDL_CreateWindow("Harvest moon", 1400, 50, MIDA_CAMERA_W, MIDA_CAMERA_H, SDL_WINDOW_SHOWN);
	//_window = SDL_CreateWindow("Harvest moon", 1000, 50, 1000, 1000, SDL_WINDOW_SHOWN);
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE);
}