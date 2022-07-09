#pragma once
#include "../../Engine/Graphics/ResourceManager.h"
#include "../../Engine/Graphics/Video.h"
#include "../Map/Map.h"
#include "ListAnimations.h"
#include "Direction.h"

class Player {

public:
	Player(Video* video, ResourceManager* rscManager);

	// GET-SET
	void setPositionX(int posX) { _posPlayerX = posX; }
	void setPositionY(int posY) { _posPlayerY = posY; }

	// Functions
	void RenderPlayer(int posX, int posY);
	void UpdatePlayer();

	// Animations
	void Animation_Walk(Direction _actualDirection);
	void Animation_Run(Direction _actualDirection);
	void Animation_Harvest(Direction _actualDirection);
	void Animation_WipeSweat();
	void Animation_Tired();
	void Animation_WornOut();
	void Animation_Drink();
	void Animation_Jump(Direction _actualDirection);
	void Animation_CarryObject(Direction _actualDirection);
	void Animation_Exercises();
	void Animation_ChangeItems(Direction _actualDirection);
	void Animation_Eat();

	int getIdRscManager() { return _idRscMngPlayer; }
	void setIdRscManager(int id) { _idRscMngPlayer = id; }

	~Player();


private:
	Video* _video;
	ResourceManager* _rscManager;
	Xywh _actualDraw;
	int _posPlayerX;
	int _posPlayerY;
	Direction _actualDirection;
	ActiveAnimation _actualAnimation;
	int _numFrameAnimation;
	int _idRscMngPlayer; // id of the graph to our ResourceManager (player, ...)

	void LoadJugador();
};

