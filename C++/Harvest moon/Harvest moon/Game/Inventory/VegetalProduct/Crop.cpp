#include "Crop.h"

void Crop::PlantSeed()
{
	_daysItTkes = 0;
	_isWatered = false;
	_actualGrowthPhase = phases.at(0);
	_canBeCollected = false;
	_actualTileToPaint = idTiles.at(0);
}

void Crop::SpendOneDay()
{
	if (_daysItTkes < daysToGrow)
		_daysItTkes++;

	_isWatered = false;
	_actualGrowthPhase = phases.at(_daysItTkes);

	if (_daysItTkes >= daysToGrow)
		_canBeCollected = true;

	_actualTileToPaint = idTiles.at(_daysItTkes * 2); // El primer és sense regar
}

void Crop::Irrigate()
{
	_isWatered = true;
}
