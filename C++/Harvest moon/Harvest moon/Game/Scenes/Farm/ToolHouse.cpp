#include "ToolHouse.h"

void ToolHouse::Init(Video* video, ResourceManager* rscManager)
{
	_videoScene = video;
	_rscManagerScene = rscManager;

	_idImageInteriorToolHouse = rscManager->loadAndGetGraphicID("Resources/Images/Edificis/Interior caseta.png");
}

void ToolHouse::ReInit()
{
	_videoScene->clearScreen();
}

void ToolHouse::Render()
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

	_videoScene->renderGraphic(_idImageInteriorToolHouse, &origin, &destination);
}

void ToolHouse::Update()
{
}
