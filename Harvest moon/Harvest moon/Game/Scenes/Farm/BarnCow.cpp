#include "BarnCow.h"

void BarnCow::Init(Video* video, ResourceManager* rscManager)
{
	_videoScene = video;
	_rscManagerScene = rscManager;
	_idImageInteriorBarnCow = rscManager->loadAndGetGraphicID("Resources/Images/Edificis/Interior estable vaca.png");
}

void BarnCow::ReInit()
{
	_videoScene->clearScreen();
}

void BarnCow::Render()
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

	_videoScene->renderGraphic(_idImageInteriorBarnCow, &origin, &destination);
}

void BarnCow::Update()
{
}
