#include "EstableVaques.h"

void EstableVaques::Init(Video* video, ResourceManager* rscManager)
{
	_videoEscena = video;
	_rscManagerEscena = rscManager;
	_idImatgeInteriorEstableVaques = rscManager->loadAndGetGraphicID("Resources/Images/Edificis/Interior estable vaca.png");
}

void EstableVaques::ReInit()
{
	_videoEscena->clearScreen();
}

void EstableVaques::Render()
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

	_videoEscena->renderGraphic(_idImatgeInteriorEstableVaques, &origen, &desti);
}

void EstableVaques::Update()
{
}
