#pragma once
#include "../Scene.h"
#include "../SceneDirector.h"

class BarnCow : public Scene {

public:

	void Init(Video* video, ResourceManager* rscManager);
	void ReInit();
	void Render();
	void Update();

	void setPlayer(Player* player) { _player = player; };
	EscenEnum getNumScene() { return _numSceneToGo; };


private:
	int _idImageInteriorBarnCow;
	Player* _player;
	EscenEnum _numSceneToGo;
};