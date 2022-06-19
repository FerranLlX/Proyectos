#pragma once
#include "../Escena.h"
#include "../DirectorEscenes.h"

class EstableVaques : public Escena {

public:

	void Init(Video* video, ResourceManager* rscManager);
	void ReInit();
	void Render();
	void Update();

	void setJugador(Jugador* jugador) { _jugador = jugador; };
	EscenaEnum getNumEscena() { return _numEscenaQueVullAnar; };


private:
	int _idImatgeInteriorEstableVaques;
	Jugador* _jugador;
	EscenaEnum _numEscenaQueVullAnar;
};