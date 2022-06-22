#include "Cultiu.h"

void Cultiu::PlantaLlavor()
{
	_diesQuePorta = 0;
	_estaRegat = false;
	_faseCreixementActual = fases.at(0);
	_esPotRecollir = false;
	_tileAPintarActual = idTiles.at(0);
}

void Cultiu::PassaUnDia()
{
	if (_diesQuePorta < diesPerCreixer)
		_diesQuePorta++;

	_estaRegat = false;
	_faseCreixementActual = fases.at(_diesQuePorta);

	if (_diesQuePorta >= diesPerCreixer)
		_esPotRecollir = true;

	_tileAPintarActual = idTiles.at(_diesQuePorta * 2); // El primer és sense regar
}

void Cultiu::Regar()
{
	_estaRegat = true;
}
