// Engine
#include "../Engine/Graphics/Video.h"
#include "../Engine/Graphics/ResourceManager.h"
#include "../Engine/Sound/SoundManager.h"
#include "../Engine/Input/InputManager.h"
#include "../Engine/Timer/Timer.h"

// Game
#include "../Game/Scenes/DirectorEscenes.h"


int main(int argc, char* args[]) {

#pragma region Inits

	Video* video = Video::getInstance();
	ResourceManager* rscManager = ResourceManager::getInstance();
	video->setResourceManager(rscManager);
	rscManager->setRender(video->getRender());

	Player* player = new Player(video, rscManager);
	DirectorEscenes* sceneDirector = DirectorEscenes::getInstance(video, rscManager, player);

	InputManager* inputManager = new InputManager();
	Keyboard* keyboard = new Keyboard();
	inputManager->InitControls(keyboard);

	Timer* timer = new Timer();

#pragma endregion


#pragma region Game loop

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
		if (keyboard->GetButtonUp(ESCAPE)) goexit = true;
		else if (keyboard->GetButtonUp(F1)) if (!key_escena1) key_escena1 = true;
		else if (keyboard->GetButtonUp(F2)) if (!key_escena2) key_escena2 = true;
		else if (keyboard->GetButtonUp(F3)) if (!key_escena3) key_escena3 = true;
		else if (keyboard->GetButtonUp(F4)) if (!key_escena4) key_escena4 = true;
		else if (keyboard->GetButtonUp(F5)) if (!key_escena5) key_escena5 = true;
		else if (keyboard->GetButtonUp(F6)) if (!key_escena6) key_escena6 = true;
		else if (keyboard->GetButtonUp(F7)) if (!key_escena7) key_escena7 = true;
		else if (keyboard->GetButtonUp(F8)) if (!key_escena8) key_escena8 = true;


		// Manual change scenes
		if (key_escena1) { sceneDirector->ChangeScene(INTRO); key_escena1 = false; }
		else if (key_escena2) { sceneDirector->ChangeScene(MAINMENU); key_escena2 = false; }
		else if (key_escena3) { sceneDirector->ChangeScene(FARM); key_escena3 = false; }
		else if (key_escena4) { sceneDirector->ChangeScene(TOWN); key_escena4 = false; }
		else if (key_escena5) { sceneDirector->ChangeScene(HOUSE); key_escena5 = false; }
		else if (key_escena6) { sceneDirector->ChangeScene(BARN_CHICKEN); key_escena6 = false; }
		else if (key_escena7) { sceneDirector->ChangeScene(BARN_COW); key_escena7 = false; }
		else if (key_escena8) { sceneDirector->ChangeScene(TOOL_SHED); key_escena8 = false; }

		// Limit FPS
		currentTime = timer->TickActual();
		deltaTime = currentTime - lastTime;
		if (deltaTime < (int)msFrame) {
			timer->Delay((int)msFrame - deltaTime);

			// Update + render actual scene
			sceneDirector->Update();
			sceneDirector->Render();
			video->updateScreen();
		}
		lastTime = currentTime;

	} while (!goexit);

#pragma endregion


#pragma region Free, delete and exit

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