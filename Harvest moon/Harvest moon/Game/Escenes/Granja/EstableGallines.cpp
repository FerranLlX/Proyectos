#include "EstableGallines.h"

void EstableGallines::Init(Video* video, ResourceManager* rscManager)
{
	_videoEscena = video;
	_rscManagerEscena = rscManager;

	_idImatgeInteriorEstableGallines = rscManager->loadAndGetGraphicID("Resources/Images/Edificis/Interior estable gallina.png");
}

void EstableGallines::ReInit()
{
	_videoEscena->clearScreen();
}

void EstableGallines::Render()
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

	_videoEscena->renderGraphic(_idImatgeInteriorEstableGallines, &origen, &desti);
}

void EstableGallines::Update()
{
}
