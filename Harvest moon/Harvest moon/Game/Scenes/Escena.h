#pragma once
#include "../../Engine/Graphics/Video.h"
#include "../../Engine/Graphics/ResourceManager.h"

class Escena {

public:
	Escena();
	// virtual: serveix per indicar que els metodes es poden redefinir en les classes heredades.	
	virtual void Init(Video* video, ResourceManager* rscManager);	// Inicialitzar l'escena.	
	virtual void ReInit();	// Fer resets de totes les variables/posicions de l'escena, si cal
	virtual void Render();	// Gestionar els dibuixos de l'escena
	virtual void Update();	// Gestionar els updates de l'escena

	virtual ~Escena();


protected:
	Video* _videoEscena;
	ResourceManager* _rscManagerEscena;
};