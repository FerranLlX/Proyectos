#pragma once

#include <SDL.h>

class Temps {

public:
	Uint32 TickActual() { return SDL_GetTicks(); }
	void Retrasar(int value);
};

