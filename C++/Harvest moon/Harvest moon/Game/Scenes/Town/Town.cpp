#include "Town.h"

void Town::Init(Video* video, ResourceManager* rscManager)
{
	_videoScene = video;
	_rscManagerScene = rscManager;

	_idImageTown = rscManager->loadAndGetGraphicID("Resources/Images/Village.png");
}

void Town::ReInit()
{
	_videoScene->clearScreen();
}

void Town::Render()
{
	Xywh origin{};
	origin.x = 0;
	origin.y = 0;
	origin.w = 160;
	origin.h = 144;

	Xywh destination{};
	destination.x = 0;
	destination.y = 0;
	destination.w = 480;
	destination.h = 432;

	_videoScene->renderGraphic(_idImageTown, &origin, &destination);
}

void Town::Update()
{
}
