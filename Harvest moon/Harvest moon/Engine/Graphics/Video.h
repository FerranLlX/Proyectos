#pragma once
#include <SDL.h>
#include "..\..\Altres\Definicions.h"
#include "ResourceManager.h"


struct Xywh
{
	int x;
	int y;
	int w;
	int h;
};


class Video
{
public:
	static Video* getInstance() {
		if (_videoInstance == NULL) _videoInstance = new Video();
		return _videoInstance;
	};

	void renderGraphic(int idTextura, Xywh* srcRect, Xywh* dstRect);
	void clearScreen();
	void pintarColorScreen(Uint32 color);
	void updateScreen();
	void waitTime(int ms);
	void close();

	SDL_Renderer* getRender() { return _renderer; };
	void setResourceManager(ResourceManager* rscManager) { _rscManager = rscManager; }

	~Video();


protected:
	Video();


private:
	static Video* _videoInstance;

	SDL_Window* _window;
	SDL_Surface* _surface;
	SDL_Renderer* _renderer;
	ResourceManager* _rscManager;
};