#pragma once
#include <string>
#include <vector>

#include "../../Adverse effects/Weather/Season.h"
#include "../../Mapa/Tileset.h"
#include "TypeSeeds.h"

class Crop {

public:
	string name;
	int price;
	int daysToGrow;
	int numPhases;
	vector<int> phases;
	vector<Season> whenToPlant;
	bool renewable;
	string nameTilemap;		// sourceTsx
	vector<int> idTiles;	// Id of each tile
	int posX;
	int posY;

	void PlantSeed();
	void SpendOneDay();
	void Irrigate();
	bool IsIrrigate() { return _isWatered; }


private:
	int _daysItTkes;
	bool _isWatered;
	int _actualGrowthPhase;
	bool _canBeCollected;
	int _actualTileToPaint;
};