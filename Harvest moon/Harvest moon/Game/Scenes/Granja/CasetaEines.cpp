#include "CasetaEines.h"

void CasetaEines::Init(Video* video, ResourceManager* rscManager)
{
	_videoEscena = video;
	_rscManagerEscena = rscManager;

	_idImatgeInteriorCaseta = rscManager->loadAndGetGraphicID("Resources/Images/Edificis/Interior caseta.png");
}

void CasetaEines::ReInit()
{
	_videoEscena->clearScreen();
}

void CasetaEines::Render()
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

	_videoEscena->renderGraphic(_idImatgeInteriorCaseta, &origen, &desti);
}

void CasetaEines::Update()
{
}
