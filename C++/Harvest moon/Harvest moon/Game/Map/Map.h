#pragma once

#include "../../_Others/Llibreries/tinyxml2.h"
#include "../../Engine/Graphics/ResourceManager.h"
#include "../../Engine/Graphics/Video.h"

#include "Layer.h"
#include "Tileset.h"
#include "../Inventory/VegetalProduct/Crop.h"
#include "../Player/Direction.h"

#include<iostream>
#include<fstream>


class Map {

public:
	static Map* getInstance(Video* video, ResourceManager* rscManager) {
		if (_mapInstance == NULL) _mapInstance = new Map(video, rscManager);
		return _mapInstance;
	};
	void setVideoManager(Video* video) { _video = video; }
	void setResourceManager(ResourceManager* rscManager) { _rscManager = rscManager; }

	// Get-set
	int getWidth() { return _width; }
	int getHeight() { return _heigth; }
	int getTileWidth() { return _tileWidth; }
	int getTileHeight() { return _tileHeight; }
	bool getActivateSquare() { return _activateSquare; }
	void setActivateSquare(bool activateSquare) { _activateSquare = activateSquare; }
	bool getActivateSquare_Irrigate() { return _activateSquare_Irrigate; }
	void setActivateSquare_Irrigate(bool activateSquare_Irrigate) { _activateSquare_Irrigate = activateSquare_Irrigate; }
	int getPosSquareX() { return _posInteract_X; }
	int getPosSquareY() { return _posInteract_Y; }

	// Functions
	void renderMap(int camX, int camY);
	void RenderSquare(int posX, int posY, int posXWithCam, int posYWithCam, Direction direction);
	int NewPositionCamX(int posPlayerX);
	int NewPositionCamY(int posPlayerY);
	bool CanPlant();
	bool CanIrrigate();
	bool ThereObstacle(int posPlayerX, int posPlayerY);
	bool ThereObstacle_East(int camPlayerX, int camPlayerY);
	bool ThereObstacle_West(int camPlayerX, int camPlayerY);
	bool ThereObstacle_North(int camPlayerX, int camPlayerY);
	bool ThereObstacle_South(int camPlayerX, int camPlayerY);
	void Carve();
	void Plant(TypeSeeds whichSeed);
	void Irrigate();
	void Manure();
	void ModifyData_PlantTomato();
	void ModifyData_Disbudding();
	void SaveMap();
	int CellContent();
	void ResetPositionSquare();

	~Map();


protected:
	Map(Video* videoManager, ResourceManager* rscManager);


private:
	static Map* _mapInstance;

	Video* _video;
	ResourceManager* _rscManager;

	// Map info
	int _width;
	int _heigth;
	int _tileWidth;
	int _tileHeight;
	vector<Layer> _layers;
	vector<Tileset> _tilesets;
	tinyxml2::XMLDocument _tmx;
	tinyxml2::XMLElement* _map;

	// Info cultius
	vector<Crop> _typesCrops;
	vector<Crop> _activeCrops;	// List of all the plants on the map

	// Camera i interacions
	int _camX;
	int _camY;
	int _idSquare_Green;
	int _idSquare_Blue;
	int _idSquare_Red;
	int _idSquare_Pink;
	bool _activateSquare;
	bool _activarRequadre_Plantar;
	bool _activateSquare_Irrigate;
	bool _activarRequadre_Abonar;
	int _posInteract_X;
	int _posInteract_Y;
	int _positionInteractDraw_X;
	int _positionInteractDraw_Y;

	// Get info
	void LoadMap();
	void GetAttributes();
	void GetTilesets();
	Tileset GetinformationAdditionalTileset(const char* sourceTempo, Tileset tilesetTempo);
	void GetLayers();
	int SearchTileset(int tileToPaint);
	void LoadExtras();
	void LoadCrops();

	// Internal functions
	int CheckCamLimitX(int camX);
	int CheckCamLimitY(int camY);
	void CalculateSquarePosition(int posX, int posY, Direction direction);
	void CalculateSquarePositionDraw(int posX, int posY, Direction direction);
	string TransformDataToString();
};