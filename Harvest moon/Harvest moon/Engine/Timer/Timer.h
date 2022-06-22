#pragma once

#include <SDL.h>

class Timer {

public:
	Uint32 TickActual() { return SDL_GetTicks(); }
	void Delay(int value);
};

