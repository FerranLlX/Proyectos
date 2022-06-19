#pragma once
#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL_events.h>
#include <vector>
#include "Keyboard.h"
#include "Controls.h"

class InputManager {

public:
	void PreHandleEvent();
	void PollEvent();
	void HandleEvent(const SDL_Event* event);
	void Update(double elapsedSeconds);
	void AddController(Gamepad* controller);

	void InitControls(Keyboard* teclat);
	
	~InputManager();


private:
	std::vector<Gamepad*> _controllers;

};

#endif