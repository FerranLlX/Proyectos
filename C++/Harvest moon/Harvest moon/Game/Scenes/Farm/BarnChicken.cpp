#include "BarnChicken.h"

void BarnChicken::Init(Video* video, ResourceManager* rscManager)
{
	_videoScene = video;
	_rscManagerScene = rscManager;

	_idImageInteriorBarnChicken = rscManager->loadAndGetGraphicID("Resources/Images/Edificis/Interior estable gallina.png");
}

void BarnChicken::ReInit()
{
	_videoScene->clearScreen();
}

void BarnChicken::Render()
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

	_videoScene->renderGraphic(_idImageInteriorBarnChicken, &origin, &destination);
}

void BarnChicken::Update()
{
}
