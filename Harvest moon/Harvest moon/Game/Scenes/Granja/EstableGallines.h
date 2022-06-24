#pragma once
#include "../Escena.h"
#include "../DirectorEscenes.h"

class EstableGallines : public Escena
{
public:

	void Init(Video* video, ResourceManager* rscManager);
	void ReInit();
	void Render();
	void Update();

	void setJugador(Player* jugador) { _jugador = jugador; };
	EscenaEnum getNumEscena() { return _numEscenaQueVullAnar; };


private:
	int _idImatgeInteriorEstableGallines;
	Player* _jugador;
	EscenaEnum _numEscenaQueVullAnar;
};