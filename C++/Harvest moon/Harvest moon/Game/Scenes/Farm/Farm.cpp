#include "Farm.h"

void Farm::Init(Video* video, ResourceManager* rscManager)
{
	_videoScene = video;
	_rscManagerScene = rscManager;

	// PENDENT: Save and/or retrieve the current day from a file
	day = 0;
	meteoActual.climaActual = SUNNY;
	meteoActual.estacioActual = SPRING;
	map = Map::getInstance(_videoScene, _rscManagerScene);
}

void Farm::ReInit()
{
	_videoScene->clearScreen();

	_worldPosX = 34;
	_worldPosY = 408;

	_actualDirection = EAST;

	_numSceneToGo = FARM;

	map->setActivateSquare(false);
	map->ResetPositionSquare();
}

void Farm::Render()
{
	// Positions world, camera, player and square
	int posCamX = map->NewPositionCamX(_worldPosX);
	int posCamY = map->NewPositionCamY(_worldPosY);
	int posPlayerX = _worldPosX - posCamX;
	int posPlayerY = _worldPosY - posCamY;

	// RENDERS
	map->renderMap(posCamX, posCamY);
	_playerFarm->RenderPlayer(posPlayerX, posPlayerY);
	map->RenderSquare(_worldPosX, _worldPosY, posPlayerX, posPlayerY, _actualDirection);
	_videoScene->UpdateScreen();
}

void Farm::Update()
{
	cout << "Enter farm" << endl;

	bool goexit = false;
	bool key_Up = false;
	bool key_Down = false;
	bool key_Right = false;
	bool key_Left = false;

	bool key_Square = false;
	bool key_Square_Irrigate = false;
	bool key_Save = false;
	bool mouse_click = false;
	bool mouse_unClick = false;

	bool plantTurnip = false;
	bool plantPotato = false;
	bool plantTomato = false;
	bool plantWheat = false;

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
					if (key_Square) key_Square = false;
					else key_Square = true;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_R)
				{
					if (key_Square_Irrigate) key_Square_Irrigate = false;
					else key_Square_Irrigate = true;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_G)
				{
					if (!key_Save)
						key_Save = true;
				}

				if (event.key.keysym.scancode == SDL_SCANCODE_1)
				{
					if (!plantTurnip) {
						plantTurnip = true;
						plantPotato = false;
						plantTomato = false;
						plantWheat = false;
						cout << "Seed turnip active!" << endl;
					}
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_2)
				{
					if (!plantPotato) {
						plantTurnip = false;
						plantPotato = true;
						plantTomato = false;
						plantWheat = false;
						cout << "Seed potato active!" << endl;
					}
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_3)
				{
					if (!plantTomato) {
						plantTurnip = false;
						plantPotato = false;
						plantTomato = true;
						plantWheat = false;
						cout << "Seed tomato active!" << endl;
					}
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_4)
				{
					if (!plantWheat) {
						plantTurnip = false;
						plantPotato = false;
						plantTomato = false;
						plantWheat = true;
						cout << "Seed wheat active!" << endl;
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
			_actualDirection = EAST;
			if (!map->ThereObstacle_East(_worldPosX, _worldPosY))
				_worldPosX++;

			WritePosition(_actualDirection, map, key_Square);
		}
		if (key_Left)
		{
			_actualDirection = WEST;
			if (!map->ThereObstacle_West(_worldPosX, _worldPosY))
				_worldPosX--;

			WritePosition(_actualDirection, map, key_Square);
		}
		if (key_Up)
		{
			_actualDirection = NORTH;
			if (!map->ThereObstacle_North(_worldPosX, _worldPosY))
				_worldPosY--;

			WritePosition(_actualDirection, map, key_Square);
		}
		if (key_Down)
		{
			_actualDirection = SOUTH;
			if (!map->ThereObstacle_South(_worldPosX, _worldPosY))
				_worldPosY++;

			WritePosition(_actualDirection, map, key_Square);
		}

		if (key_Square) {
			map->setActivateSquare(true);
			map->setActivateSquare_Irrigate(false);
		}
		else map->setActivateSquare(false);

		if (key_Square_Irrigate) {
			map->setActivateSquare(false);
			map->setActivateSquare_Irrigate(true);
		}
		else map->setActivateSquare_Irrigate(false);

		if (key_Save)
		{
			map->SaveMap();
			cout << "Game saved!" << endl;
			key_Save = false;
		}
		if (mouse_click)
		{
			if (map->getActivateSquare())
			{
				if (plantTurnip) {
					map->Plant(TURNIP);
					cout << "TURNIP planted!" << endl;
				}
				else if (plantPotato) {
					map->Plant(POTATO);
					cout << "POTATO planted!" << endl;
				}
				else if (plantTomato) {
					map->Plant(TOMATO);
					cout << "TOMATO planted!" << endl;
				}
				else if (plantWheat) {
					map->Plant(WHEAT);
					cout << "WHEAT planted!" << endl;
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
			cout << "Unplanted!" << endl;
			mouse_unClick = false;
		}


		// SPECIALS CELLS
		int contentCell = map->CellContent();

		// Business container
		if (contentCell == 295)
		{
			// TODO : Increasing money according to the product
			
		}

		// Change scenes
		if (contentCell == 290)
		{
			goexit = true;
			_numSceneToGo = TOWN;
			cout << "You are going to town" << endl;
		}
		else if (contentCell == 291)
		{
			goexit = true;
			_numSceneToGo = HOUSE;
			cout << "You go home" << endl;
		}
		else if (contentCell == 292)
		{
			goexit = true;
			_numSceneToGo = BARN_COW;
			cout << "You are going to cow stable " << endl;
		}
		else if (contentCell == 293)
		{
			goexit = true;
			_numSceneToGo = BARN_CHICKEN;
			cout << "You are going to chicken stable" << endl;
		}
		else if (contentCell == 294)
		{
			goexit = true;
			_numSceneToGo = TOOL_SHED;
			cout << "You are going to tool house" << endl;
		}
		else
			Render();

	} while (!goexit);

	cout << "Farm out" << endl;
}

void Farm::WritePosition(Direction direction, Map* map, bool active)
{
	switch (direction)
	{
	case NORTH: cout << "PlayerX:" << _worldPosX << ", PlayerY:" << _worldPosY << ", Dir:" << "NORTH"; break;
	case SOUTH: cout << "PlayerX:" << _worldPosX << ", PlayerY:" << _worldPosY << ", Dir:" << "SOUTH"; break;
	case WEST: cout << "PlayerX:" << _worldPosX << ", PlayerY:" << _worldPosY << ", Dir:" << "WEST"; break;
	case EAST: cout << "PlayerX:" << _worldPosX << ", PlayerY:" << _worldPosY << ", Dir:" << "EAST"; break;
	default: break;
	}

	if (active)
		cout << ", SquareX:" << map->getPosSquareX() << ", SquareY:" << map->getPosSquareY() << endl;
	else
		cout << endl;
}