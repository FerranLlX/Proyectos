#pragma once
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "Gamepad.h"
#include <unordered_map>

class Keyboard : public Gamepad {

public:
	void AddButtonMap(InputButton& inputButton);
	void AddAxisMap(InputAxis& inputAxis);
	void HandleEvent(const SDL_Event* event) override;
	bool GetButtonDown(int actionId) const override;
	bool GetButtonUp(int actionId) const override;
	bool GetButton(int actionId) const override;
	double GetAxis(const int actionId) const override;
	void ResetEvents() override;

	~Keyboard();


private:
	void HandleButton(const SDL_Event* event);
	void HandleAxis(const SDL_Event* event);

	std::unordered_map<int, InputButton> _buttonsMap;
	std::unordered_map<SDL_Keycode, int> _keyToActionMap;

	std::unordered_map<int, InputAxis> _axisMap;
	std::unordered_map<SDL_Keycode, int> _keyToAxisActionMap;
};

#endif