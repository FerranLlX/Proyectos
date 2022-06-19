// Engine
#include "../Engine/Graphics/Video.h"
#include "../Engine/Graphics/ResourceManager.h"
#include "../Engine/Sound/SoundManager.h"
#include "../Engine/Input/InputManager.h"
#include "../Engine/Timer/Temps.h"

// Game
#include "../Game/Escenes/DirectorEscenes.h"


int main(int argc, char* args[]) {

#pragma region Inicialitzacions

	Video* video = Video::getInstance();
	ResourceManager* rscManager = ResourceManager::getInstance();
	video->setResourceManager(rscManager);
	rscManager->setRender(video->getRender());

	Jugador* jugador = new Jugador(video, rscManager);
	DirectorEscenes* directorEscenes = DirectorEscenes::getInstance(video, rscManager, jugador);

	InputManager* inputManager = new InputManager();
	Keyboard* teclat = new Keyboard();
	inputManager->InitControls(teclat);

	Temps* tempo = new Temps();

#pragma endregion


#pragma region Bucle del joc

	bool goexit = false;
	bool key_escena1 = false;	// F1
	bool key_escena2 = false;	// F2
	bool key_escena3 = false;	// F3
	bool key_escena4 = false;	// F4
	bool key_escena5 = false;	// F5
	bool key_escena6 = false;	// F6
	bool key_escena7 = false;	// F7
	bool key_escena8 = false;	// F8
	bool key_escena9 = false;	// F9
	bool key_escena10 = false;	// F10
	bool key_escena11 = false;	// F11
	bool key_escena12 = false;	// F12

	unsigned int lastTime = 0, currentTime, deltaTime;
	float msFrame = 1 / (FPS / 1000.0f);

	do {
		inputManager->PreHandleEvent();
		inputManager->PollEvent();

		// Key input
		if (teclat->GetButtonUp(ESCAPE)) goexit = true;
		else if (teclat->GetButtonUp(F1)) if (!key_escena1) key_escena1 = true;
		else if (teclat->GetButtonUp(F2)) if (!key_escena2) key_escena2 = true;
		else if (teclat->GetButtonUp(F3)) if (!key_escena3) key_escena3 = true;
		else if (teclat->GetButtonUp(F4)) if (!key_escena4) key_escena4 = true;
		else if (teclat->GetButtonUp(F5)) if (!key_escena5) key_escena5 = true;
		else if (teclat->GetButtonUp(F6)) if (!key_escena6) key_escena6 = true;
		else if (teclat->GetButtonUp(F7)) if (!key_escena7) key_escena7 = true;
		else if (teclat->GetButtonUp(F8)) if (!key_escena8) key_escena8 = true;


		// CANVI MANUAL D'ESCENES
		if (key_escena1) { directorEscenes->CanviEscena(INTRO);	key_escena1 = false; }
		else if (key_escena2) { directorEscenes->CanviEscena(MAINMENU); key_escena2 = false; }
		else if (key_escena3) { directorEscenes->CanviEscena(GRANJA); key_escena3 = false; }
		else if (key_escena4) { directorEscenes->CanviEscena(POBLE); key_escena4 = false; }
		else if (key_escena5) { directorEscenes->CanviEscena(CASA); key_escena5 = false; }
		else if (key_escena6) { directorEscenes->CanviEscena(ESTABLE_GALLINES); key_escena6 = false; }
		else if (key_escena7) { directorEscenes->CanviEscena(ESTABLE_VAQUES); key_escena7 = false; }
		else if (key_escena8) { directorEscenes->CanviEscena(CASETA); key_escena8 = false; }

		// Limitar FPS
		currentTime = tempo->TickActual();
		deltaTime = currentTime - lastTime;
		if (deltaTime < (int)msFrame) {
			tempo->Retrasar((int)msFrame - deltaTime);

			// Update + render de l'escena actual
			directorEscenes->Update();
			directorEscenes->Render();
			video->updateScreen();
		}
		lastTime = currentTime;

	} while (!goexit);

#pragma endregion


#pragma region Alliberar, Borrar i Sortir

	// Alliberar totes les instancies i singletons	
	inputManager->~InputManager();
	rscManager->~ResourceManager();
	video->~Video();

	delete inputManager;
	delete rscManager;
	delete video;

	SDL_Quit();

	return 0;
#pragma endregion
}