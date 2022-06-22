#include "Poble.h"

void Poble::Init(Video* video, ResourceManager* rscManager)
{
	_videoEscena = video;
	_rscManagerEscena = rscManager;

	_idImatgePoble = rscManager->loadAndGetGraphicID("Resources/Images/Village.png");
}

void Poble::ReInit()
{
	_videoEscena->clearScreen();
}

void Poble::Render()
{
	Xywh origen{};
	origen.x = 0;
	origen.y = 0;
	origen.w = 160;
	origen.h = 144;

	Xywh desti{};
	desti.x = 0;
	desti.y = 0;
	desti.w = 480;
	desti.h = 432;

	_videoEscena->renderGraphic(_idImatgePoble, &origen, &desti);
}

void Poble::Update()
{
}
