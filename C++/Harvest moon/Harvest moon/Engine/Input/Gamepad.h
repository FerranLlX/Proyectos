#pragma once
#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <SDL_events.h>

enum ButtonStates {
	None,
	Pressed,
	Released
};

struct InputButton {
	int ActionId;
	int Code;
	ButtonStates State;

	InputButton() : ActionId(0), Code(0), State(ButtonStates::None) {}

	InputButton(int actionId, int code) : ActionId(actionId), Code(code), State(ButtonStates::None) {
	}
};

struct InputAxis {
	int ActionId;
	int NegativeCode;
	int PositiveCode;
	int Value;

	InputAxis() : ActionId(0), NegativeCode(-1), PositiveCode(1), Value(0) {}

	InputAxis(int actionId, int negativeCode, int positiveCode) : ActionId(actionId), NegativeCode(negativeCode),
		PositiveCode(positiveCode), Value(0) {}
};

class Gamepad {

public:
	virtual void HandleEvent(const SDL_Event* event) = 0;
	virtual bool GetButtonDown(int actionId) const = 0;
	virtual bool GetButtonUp(int actionId) const = 0;
	virtual bool GetButton(int actionId) const = 0;
	virtual double GetAxis(const int actionId) const = 0;


	virtual void ResetEvents() = 0;
};

#endif