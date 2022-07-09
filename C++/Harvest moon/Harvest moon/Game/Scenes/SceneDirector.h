#pragma once
#include <vector>
#include <iostream>

#include "../../Engine/Graphics/Video.h"
#include "../../Engine/Graphics/ResourceManager.h"
#include "../Player/Player.h"
#include "Scene.h"
#include "ListScenes.h"


#define NUM_SCENES _LASTSCENE
using namespace std;

class SceneDirector {

public:
	static SceneDirector* getInstance(Video* video, ResourceManager* rscManager, Player* player) {
		if (_directorScenesInstance == NULL) _directorScenesInstance = new SceneDirector(video, rscManager, player);
		return _directorScenesInstance;
	};  // Gets Singleton instance}
		
	void Update();
	void Render();
	void ChangeScene(EscenEnum nextScene);

	EscenEnum getActualScene() { return _actualScene; }
	void setActualScene(EscenEnum numScene) { _actualScene = numScene; }

	Scene* getEscenaActual() { return _vectorScenes[_actualScene]; }	
	void NameActualScene();	

	~SceneDirector();


protected:
	SceneDirector(Video* video, ResourceManager* rscManager, Player* player);


private:
	static SceneDirector* _directorScenesInstance;	// Singleton instance
		
	std::vector<Scene*> _vectorScenes;
	
	EscenEnum _actualScene;
};