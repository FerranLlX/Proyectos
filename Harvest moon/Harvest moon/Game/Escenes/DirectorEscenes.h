#pragma once
#include <vector>
#include <iostream>

#include "../../Engine/Graphics/Video.h"
#include "../../Engine/Graphics/ResourceManager.h"
#include "../Jugador/Jugador.h"
#include "Escena.h"
#include "LlistaEscenes.h"


#define NUM_SCENES _LASTSCENE
using namespace std;

class DirectorEscenes {

public:
	static DirectorEscenes* getInstance(Video* video, ResourceManager* rscManager, Jugador* jugador) {
		if (_directorEscenesInstance == NULL) _directorEscenesInstance = new DirectorEscenes(video, rscManager, jugador);
		return _directorEscenesInstance;
	};  // Gets Singleton instance}
		
	void Update();
	void Render();
	void CanviEscena(EscenaEnum seguentEscena);

	EscenaEnum getEscenaQueToca() { return _escenaActual; }
	void setEscenaQueToca(EscenaEnum numEscena) { _escenaActual = numEscena; }

	Escena* getEscenaActual() { return _vectorEscenes[_escenaActual]; }	
	void NomEscenaActual();	

	~DirectorEscenes();


protected:
	DirectorEscenes(Video* video, ResourceManager* rscManager, Jugador* jugador);


private:
	static DirectorEscenes* _directorEscenesInstance;	// Singleton instance
		
	std::vector<Escena*> _vectorEscenes;
	
	EscenaEnum _escenaActual;
};