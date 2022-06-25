#include "House.h"

void House::Init(Video* video, ResourceManager* rscManager)
{	
	_videoScene = video;
	_rscManagerScene = rscManager;

	_idImageInteriorHouse = rscManager->loadAndGetGraphicID("Resources/Images/Edificis/Interior casa.png");
}

void House::ReInit()
{
	_videoScene->clearScreen();
}

void House::Render()
{
	Xywh origin{};
	origin.x = 0;
	origin.y = 0;
	origin.w = 480;
	origin.h = 420;

	Xywh destination{};
	destination.x = 0;
	destination.y = 0;
	destination.w = 480;
	destination.h = 420;

	_videoScene->renderGraphic(_idImageInteriorHouse, &origin, &destination);
}

void House::Update()
{
}
