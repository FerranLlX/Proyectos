#include "Granja.h"

void Granja::Init(Video* video, ResourceManager* rscManager)
{
	_videoEscena = video;
	_rscManagerEscena = rscManager;

	// PENDENT: Guardar i/o obtenir el dia actual desde un fitxer
	dia = 0;
	meteoActual.climaActual = SUNNY;
	meteoActual.estacioActual = SPRING;
	map = Map::getInstance(_videoEscena, _rscManagerEscena);
}

void Granja::ReInit()
{
	_videoEscena->clearScreen();

	_posMonX = 34;
	_posMonY = 408;

	_dirActual = EAST;

	_numEscenaQueVullAnar = FARM;

	map->setActivateSquare(false);
	map->ResetPositionSquare();
}

void Granja::Render()
{
	// Posicions Mon, Camera, Jugador i Requadre
	int posCamX = map->NewPositionCamX(_posMonX);
	int posCamY = map->NewPositionCamY(_posMonY);
	int posJugadorX = _posMonX - posCamX;
	int posJugadorY = _posMonY - posCamY;

	// RENDERS
	map->renderMap(posCamX, posCamY);
	_playerFarm->RenderJugador(posJugadorX, posJugadorY);
	map->RenderSquare(_posMonX, _posMonY, posJugadorX, posJugadorY, _dirActual);
	_videoEscena->updateScreen();
}

void Granja::Update()
{
	cout << "Entra a granja" << endl;

	bool goexit = false;
	bool key_Up = false;
	bool key_Down = false;
	bool key_Right = false;
	bool key_Left = false;

	bool key_Requadre = false;
	bool key_Requadre_Regar = false;
	bool key_Guardar = false;
	bool mouse_click = false;
	bool mouse_unClick = false;

	bool plantarNap = false;
	bool plantarPatata = false;
	bool plantarTomata = false;
	bool plantarBlat = false;

	do {
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type) {

			case SDL_KEYDOWN:
				if (event.key.keysym.scancode == SDL_SCANCODE_UP || event.key.keysym.scancode == SDL_SCANCODE_W) {
					key_Up = true;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_DOWN || event.key.keysym.scancode == SDL_SCANCODE_S) {
					key_Down = true;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT || event.key.keysym.scancode == SDL_SCANCODE_D) {
					key_Right = true;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_LEFT || event.key.keysym.scancode == SDL_SCANCODE_A) {
					key_Left = true;
				}
				break;

			case SDL_KEYUP:
				if (event.key.keysym.scancode == SDL_SCANCODE_UP || event.key.keysym.scancode == SDL_SCANCODE_W) { key_Up = false; }
				if (event.key.keysym.scancode == SDL_SCANCODE_DOWN || event.key.keysym.scancode == SDL_SCANCODE_S) { key_Down = false; }
				if (event.key.keysym.scancode == SDL_SCANCODE_RIGHT || event.key.keysym.scancode == SDL_SCANCODE_D) { key_Right = false; }
				if (event.key.keysym.scancode == SDL_SCANCODE_LEFT || event.key.keysym.scancode == SDL_SCANCODE_A) { key_Left = false; }
				if (event.key.keysym.scancode == SDL_SCANCODE_E)
				{
					if (key_Requadre) key_Requadre = false;
					else key_Requadre = true;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_R)
				{
					if (key_Requadre_Regar) key_Requadre_Regar = false;
					else key_Requadre_Regar = true;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_G)
				{
					if (!key_Guardar)
						key_Guardar = true;
				}

				if (event.key.keysym.scancode == SDL_SCANCODE_1)
				{
					if (!plantarNap) {
						plantarNap = true;
						plantarPatata = false;
						plantarTomata = false;
						plantarBlat = false;
						cout << "Llavors nap actiu!" << endl;
					}
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_2)
				{
					if (!plantarPatata) {
						plantarNap = false;
						plantarPatata = true;
						plantarTomata = false;
						plantarBlat = false;
						cout << "Llavors patata actiu!" << endl;
					}
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_3)
				{
					if (!plantarTomata) {
						plantarNap = false;
						plantarPatata = false;
						plantarTomata = true;
						plantarBlat = false;
						cout << "Llavors tomata actiu!" << endl;
					}
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_4)
				{
					if (!plantarBlat) {
						plantarNap = false;
						plantarPatata = false;
						plantarTomata = false;
						plantarBlat = true;
						cout << "Llavors blat actiu!" << endl;
					}
				}
				break;

			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
				{
					mouse_click = true;
				}
				if (event.button.button == SDL_BUTTON_RIGHT)
				{
					mouse_unClick = true;
				}
				break;

			default:
				break;
			}
		}

		if (key_Right)
		{
			_dirActual = EAST;
			if (!map->ThereObstacle_East(_posMonX, _posMonY))
				_posMonX++;

			EscriurePosicio(_dirActual, map, key_Requadre);
		}
		if (key_Left)
		{
			_dirActual = WEST;
			if (!map->ThereObstacle_West(_posMonX, _posMonY))
				_posMonX--;

			EscriurePosicio(_dirActual, map, key_Requadre);
		}
		if (key_Up)
		{
			_dirActual = NORTH;
			if (!map->ThereObstacle_North(_posMonX, _posMonY))
				_posMonY--;

			EscriurePosicio(_dirActual, map, key_Requadre);
		}
		if (key_Down)
		{
			_dirActual = SOUTH;
			if (!map->ThereObstacle_South(_posMonX, _posMonY))
				_posMonY++;

			EscriurePosicio(_dirActual, map, key_Requadre);
		}

		if (key_Requadre) {
			map->setActivateSquare(true);
			map->setActivateSquare_Irrigate(false);
		}
		else map->setActivateSquare(false);

		if (key_Requadre_Regar) {
			map->setActivateSquare(false);
			map->setActivateSquare_Irrigate(true);
		}
		else map->setActivateSquare_Irrigate(false);

		if (key_Guardar)
		{
			map->SaveMap();
			cout << "Partida guardada!" << endl;
			key_Guardar = false;
		}
		if (mouse_click)
		{
			if (map->getActivateSquare())
			{
				if (plantarNap) {
					map->Plant(TURNIP);
					cout << "NAP plantat!" << endl;
				}
				else if (plantarPatata) {
					map->Plant(POTATO);
					cout << "PATATA plantat!" << endl;
				}
				else if (plantarTomata) {
					map->Plant(TOMATO);
					cout << "TOMATA plantat!" << endl;
				}
				else if (plantarBlat) {
					map->Plant(WHEAT);
					cout << "BLAT plantat!" << endl;
				}
			}
			else if (map->getActivateSquare_Irrigate())
			{
				map->Irrigate();
			}
			
			mouse_click = false;
		}
		if (mouse_unClick)
		{
			map->ModifyData_Disbudding();
			cout << "Desplantat!" << endl;
			mouse_unClick = false;
		}


		// CELES ESPECIALS
		int contigutCela = map->CellContent();

		// Contenidor negocis
		if (contigutCela == 295)
		{
			// TODO : Incrementar diners segons el producte
			cout << "Toques contenidor" << endl;
		}

		// Canvis d'escenes
		if (contigutCela == 290)
		{
			goexit = true;
			_numEscenaQueVullAnar = TOWN;
			cout << "Vas a poble" << endl;
		}
		else if (contigutCela == 291)
		{
			goexit = true;
			_numEscenaQueVullAnar = HOUSE;
			cout << "Vas a casa" << endl;
		}
		else if (contigutCela == 292)
		{
			goexit = true;
			_numEscenaQueVullAnar = BARN_COW;
			cout << "Vas a vaques" << endl;
		}
		else if (contigutCela == 293)
		{
			goexit = true;
			_numEscenaQueVullAnar = BARN_CHICKEN;
			cout << "Vas a gallines" << endl;
		}
		else if (contigutCela == 294)
		{
			goexit = true;
			_numEscenaQueVullAnar = TOOL_SHED;
			cout << "Vas a caseta" << endl;
		}
		else
			Render();

	} while (!goexit);

	cout << "Surt de granja" << endl;
}

void Granja::EscriurePosicio(Direction direcio, Map* map, bool actiu)
{
	switch (direcio)
	{
	case NORTH: cout << "JugadorX:" << _posMonX << ", JugadorY:" << _posMonY << ", Dir:" << "NORD"; break;
	case SOUTH: cout << "JugadorX:" << _posMonX << ", JugadorY:" << _posMonY << ", Dir:" << "SUD"; break;
	case WEST: cout << "JugadorX:" << _posMonX << ", JugadorY:" << _posMonY << ", Dir:" << "OEST"; break;
	case EAST: cout << "JugadorX:" << _posMonX << ", JugadorY:" << _posMonY << ", Dir:" << "EST"; break;
	default: break;
	}

	if (actiu)
		cout << ", RequadreX:" << map->getPosSquareX() << ", RequadreY:" << map->getPosSquareY() << endl;
	else
		cout << endl;
}