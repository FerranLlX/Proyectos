#include "InputManager.h"

InputManager::~InputManager()
{
}

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

void InputManager::InitControls(Keyboard* teclat)
{
	InputButton b_escape = InputButton{ ESCAPE, SDLK_ESCAPE };

	InputButton b_up = InputButton{ UP, SDLK_w };
	InputButton b_down = InputButton{ DOWN, SDLK_s };
	InputButton b_left = InputButton{ LEFT, SDLK_a };
	InputButton b_right = InputButton{ RIGHT, SDLK_d };
	InputButton b_interactuar = InputButton{ INTERACTUAR, SDLK_e };
	InputButton b_regar = InputButton{ REGAR, SDLK_r };
	InputButton b_guardar = InputButton{ GUARDAR, SDLK_g };

	InputButton b_canviEscena_1 = InputButton{ F1, SDLK_F1 };
	InputButton b_canviEscena_2 = InputButton{ F2, SDLK_F2 };
	InputButton b_canviEscena_3 = InputButton{ F3, SDLK_F3 };
	InputButton b_canviEscena_4 = InputButton{ F4, SDLK_F4 };
	InputButton b_canviEscena_5 = InputButton{ F5, SDLK_F5 };
	InputButton b_canviEscena_6 = InputButton{ F6, SDLK_F6 };
	InputButton b_canviEscena_7 = InputButton{ F7, SDLK_F7 };
	InputButton b_canviEscena_8 = InputButton{ F8, SDLK_F8 };

	InputButton b_llavor_1 = InputButton{ LLAVOR1, SDLK_1 };
	InputButton b_llavor_2 = InputButton{ LLAVOR2, SDLK_2 };
	InputButton b_llavor_3 = InputButton{ LLAVOR3, SDLK_3 };
	InputButton b_llavor_4 = InputButton{ LLAVOR4, SDLK_4 };

	InputButton b_mouse_1 = InputButton{ ACEPTAR, SDL_BUTTON_LEFT };
	InputButton b_mouse_2 = InputButton{ CANCELAR, SDL_BUTTON_RIGHT };


	teclat->AddButtonMap(b_escape);

	teclat->AddButtonMap(b_up);
	teclat->AddButtonMap(b_down);
	teclat->AddButtonMap(b_left);
	teclat->AddButtonMap(b_right);
	teclat->AddButtonMap(b_interactuar);
	teclat->AddButtonMap(b_regar);
	teclat->AddButtonMap(b_guardar);

	teclat->AddButtonMap(b_canviEscena_1);
	teclat->AddButtonMap(b_canviEscena_2);
	teclat->AddButtonMap(b_canviEscena_3);
	teclat->AddButtonMap(b_canviEscena_4);
	teclat->AddButtonMap(b_canviEscena_5);
	teclat->AddButtonMap(b_canviEscena_6);
	teclat->AddButtonMap(b_canviEscena_7);
	teclat->AddButtonMap(b_canviEscena_8);

	teclat->AddButtonMap(b_llavor_1);
	teclat->AddButtonMap(b_llavor_2);
	teclat->AddButtonMap(b_llavor_3);
	teclat->AddButtonMap(b_llavor_4);

	teclat->AddButtonMap(b_mouse_1);
	teclat->AddButtonMap(b_mouse_2);


	AddController(teclat);
}
