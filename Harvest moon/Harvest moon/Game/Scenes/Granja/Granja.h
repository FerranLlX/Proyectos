#pragma once
#include "../../Adverse effects/Weather/Weather.h"
#include "../../Mapa/Mapa.h"
#include "../../Jugador/Jugador.h";
#include "../Escena.h"
#include "../DirectorEscenes.h"


class Granja : public Escena {

public:
	int dia;
	Weather meteoActual;
	Mapa* map;

	void Init(Video* video, ResourceManager* rscManager);
	void ReInit();
	void Update();
	void Render();

	void setJugador(Jugador* jugador) { _jugadorGranja = jugador; };
	EscenaEnum getNumEscena() { return _numEscenaQueVullAnar; };


private:
	int _posMonX;
	int _posMonY;

	Direcio _dirActual;
	Jugador* _jugadorGranja;
	EscenaEnum _numEscenaQueVullAnar;	

	void EscriurePosicio(Direcio direcio, Mapa* map, bool actiu);
};