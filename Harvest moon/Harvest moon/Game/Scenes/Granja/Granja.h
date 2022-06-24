#pragma once
#include "../../Adverse effects/Weather/Weather.h"
#include "../../Mapa/Mapa.h"
#include "../../Player/Player.h";
#include "../Escena.h"
#include "../DirectorEscenes.h"


class Granja : public Escena {

public:
	int dia;
	Weather meteoActual;
	Map* map;

	void Init(Video* video, ResourceManager* rscManager);
	void ReInit();
	void Update();
	void Render();

	void setJugador(Player* jugador) { _playerFarm = jugador; };
	EscenaEnum getNumEscena() { return _numEscenaQueVullAnar; };


private:
	int _posMonX;
	int _posMonY;

	Direction _dirActual;
	Player* _playerFarm;
	EscenaEnum _numEscenaQueVullAnar;	

	void EscriurePosicio(Direction direcio, Map* map, bool actiu);
};