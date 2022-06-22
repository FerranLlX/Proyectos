#include "Casa.h"

void Casa::Init(Video* video, ResourceManager* rscManager)
{	
	_videoEscena = video;
	_rscManagerEscena = rscManager;

	_idImatgeInteriorCasa = rscManager->loadAndGetGraphicID("Resources/Images/Edificis/Interior casa.png");
}

void Casa::ReInit()
{
	_videoEscena->clearScreen();
}

void Casa::Render()
{
	Xywh origen{};
	origen.x = 0;
	origen.y = 0;
	origen.w = 480;
	origen.h = 420;

	Xywh desti{};
	desti.x = 0;
	desti.y = 0;
	desti.w = 480;
	desti.h = 420;

	_videoEscena->renderGraphic(_idImatgeInteriorCasa, &origen, &desti);
}

void Casa::Update()
{
}
