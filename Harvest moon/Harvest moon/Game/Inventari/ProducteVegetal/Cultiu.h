#pragma once
#include <string>
#include <vector>

#include "../../Efectes adversos/Temps/Estacio.h"
#include "../../Mapa/Tileset.h"
#include "Llavor.h"

class Cultiu {

public:
	string nom;
	int preu;
	int diesPerCreixer;
	int numFases;
	vector<int> fases;
	vector<Estacio> quanPlantar;
	bool renovable;
	string nomTilemap;		// sourceTsx
	vector<int> idTiles;	// Id de cada tile
	int posX;
	int posY;

	void PlantaLlavor();
	void PassaUnDia();
	void Regar();
	bool EstaRegat() { return _estaRegat; }


private:
	int _diesQuePorta;
	bool _estaRegat;
	int _faseCreixementActual;
	bool _esPotRecollir;
	int _tileAPintarActual;
};