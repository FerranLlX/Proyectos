#include "InputManager.h"

InputManager::~InputManager() {}

void InputManager::PreHandleEvent() {
	for (const auto& controller : _controllers) {
		controller->ResetEvents();
	}
}

void InputManager::PollEvent()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			//_isRunning = false;
		}
		else {
			HandleEvent(&event);
		}
	}
}

void InputManager::HandleEvent(const SDL_Event* event) {

	for (const auto& controller : _controllers) {
		controller->HandleEvent(event);
	}
}

void InputManager::Update(double elapsedSeconds)
{
}

void InputManager::AddController(Gamepad* controller) {
	_controllers.push_back(controller);
}

void InputManager::InitControls(Keyboard* keyboard)
{
	InputButton b_escape = InputButton{ ESCAPE, SDLK_ESCAPE };

	InputButton b_up = InputButton{ UP, SDLK_w };
	InputButton b_down = InputButton{ DOWN, SDLK_s };
	InputButton b_left = InputButton{ LEFT, SDLK_a };
	InputButton b_right = InputButton{ RIGHT, SDLK_d };
	InputButton b_interact = InputButton{ INTERACT, SDLK_e };
	InputButton b_irrigate = InputButton{ IRRIGATE, SDLK_r };
	InputButton b_save = InputButton{ SAVE, SDLK_g };

	InputButton b_changeScene_1 = InputButton{ F1, SDLK_F1 };
	InputButton b_changeScene_2 = InputButton{ F2, SDLK_F2 };
	InputButton b_changeScene_3 = InputButton{ F3, SDLK_F3 };
	InputButton b_changeScene_4 = InputButton{ F4, SDLK_F4 };
	InputButton b_changeScene_5 = InputButton{ F5, SDLK_F5 };
	InputButton b_changeScene_6 = InputButton{ F6, SDLK_F6 };
	InputButton b_changeScene_7 = InputButton{ F7, SDLK_F7 };
	InputButton b_changeScene_8 = InputButton{ F8, SDLK_F8 };

	InputButton b_seed_1 = InputButton{ SEED1, SDLK_1 };
	InputButton b_seed_2 = InputButton{ SEED2, SDLK_2 };
	InputButton b_seed_3 = InputButton{ SEED3, SDLK_3 };
	InputButton b_seed_4 = InputButton{ SEED4, SDLK_4 };

	InputButton b_mouse_1 = InputButton{ ACCEPT, SDL_BUTTON_LEFT };
	InputButton b_mouse_2 = InputButton{ CANCEL, SDL_BUTTON_RIGHT };


	keyboard->AddButtonMap(b_escape);

	keyboard->AddButtonMap(b_up);
	keyboard->AddButtonMap(b_down);
	keyboard->AddButtonMap(b_left);
	keyboard->AddButtonMap(b_right);
	keyboard->AddButtonMap(b_interact);
	keyboard->AddButtonMap(b_irrigate);
	keyboard->AddButtonMap(b_save);

	keyboard->AddButtonMap(b_changeScene_1);
	keyboard->AddButtonMap(b_changeScene_2);
	keyboard->AddButtonMap(b_changeScene_3);
	keyboard->AddButtonMap(b_changeScene_4);
	keyboard->AddButtonMap(b_changeScene_5);
	keyboard->AddButtonMap(b_changeScene_6);
	keyboard->AddButtonMap(b_changeScene_7);
	keyboard->AddButtonMap(b_changeScene_8);

	keyboard->AddButtonMap(b_seed_1);
	keyboard->AddButtonMap(b_seed_2);
	keyboard->AddButtonMap(b_seed_3);
	keyboard->AddButtonMap(b_seed_4);

	keyboard->AddButtonMap(b_mouse_1);
	keyboard->AddButtonMap(b_mouse_2);


	AddController(keyboard);
}
