#pragma once
#include "../../Adverse effects/Weather/Weather.h"
#include "../../Map/Map.h"
#include "../../Player/Player.h";
#include "../Scene.h"
#include "../SceneDirector.h"


class Farm : public Scene {

public:
	int day;
	Weather meteoActual;
	Map* map;

	void Init(Video* video, ResourceManager* rscManager);
	void ReInit();
	void Update();
	void Render();

	void setPlayer(Player* player) { _playerFarm = player; };
	EscenEnum getNumScene() { return _numSceneToGo; };


private:
	int _worldPosX;
	int _worldPosY;

	Direction _actualDirection;
	Player* _playerFarm;
	EscenEnum _numSceneToGo;	

	void WritePosition(Direction direction, Map* map, bool active);
};