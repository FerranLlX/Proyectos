#include "Map.h"

Map* Map::_mapInstance = NULL;


#pragma region Public

void Map::renderMap(int camX, int camY)
{
	_camX = camX;
	_camY = camY;

	int initCamX = _camX / _tileWidth;
	int initCamY = _camY / _tileHeight;
	int finalCamX = (SIZE_CAMERA_W / _tileWidth) + initCamX;
	int finalCamY = (SIZE_CAMERA_H / _tileHeight) + initCamY;

	int numLayers = _layers.size();
	for (int i = 0; i < numLayers - 1; i++) // - 1 because the last level is logic level (access / doors / containers)
	{
		// Get data from the current layer
		vector<vector<int>>layerData = _layers.at(i).data;

		int posCamX = 0;
		int posCamY = 0;

		for (int y = initCamY; y < finalCamY; y++)
		{
			for (int x = initCamX; x < finalCamX; x++)
			{
				// Obtenir tile
				int tileToPaint = layerData[x][y];

				if (tileToPaint != 0)
				{
					// Buscar tileset que tingui el tile a pintar					
					int whichTilesetOfVector = SearchTileset(tileToPaint);
					if (whichTilesetOfVector != -1)
					{
						Tileset tilesetToWork = _tilesets.at(whichTilesetOfVector);

						// Buscar id del rscManager
						int idTileset = tilesetToWork.getIdRscManager();

						// Comprobar el tileAPintar segons el tileset
						tileToPaint = tileToPaint - tilesetToWork.getFirstgid() + 1;

						// Mida i posicio del tiles del tileset
						Xywh SrcR{};
						Xywh DestR{};

						int posYTile = -1;
						int posXTile = -1;

						int counter = 1;
						bool found = false;
						for (int y = 0; y < tilesetToWork.getRows(); y++)
						{
							for (int x = 0; x < tilesetToWork.getColumns(); x++)
							{
								if (counter == tileToPaint)
								{
									posXTile = x;
									posYTile = y;
									found = true;
									break;
								}
								else
									counter++;
							}
							if (found)
								break;
						}

						// Multiply by tile size
						posYTile *= _tileHeight;
						posXTile *= _tileWidth;

						// Assign  RECTs SRC-DEST
						SrcR.x = posXTile;
						SrcR.y = posYTile;
						SrcR.w = _tileWidth;
						SrcR.h = _tileHeight;

						DestR.x = x * _tileWidth;
						DestR.y = y * _tileHeight;
						DestR.w = _tileWidth;
						DestR.h = _tileHeight;

						// Smooth the camera because it does not make jumps of 16 by 16
						DestR.x = DestR.x - _camX;
						DestR.y = DestR.y - _camY;

						// Paint tile(of tileset) to render						
						_video->renderGraphic(idTileset, &SrcR, &DestR);
					}
				}
				posCamX++;
			}
			posCamX = 0;
			posCamY++;
		}
	}
}

void Map::RenderSquare(int posX, int posY, int posXWithCam, int posYWithCam, Direction direction)
{
	if (_activateSquare)
	{
		// Square with world position, real place where to plant
		CalculateSquarePosition(posX, posY, direction);
		// Square drawing position Player/Camera
		CalculateSquarePositionDraw(posXWithCam, posYWithCam, direction);

		Xywh origin{};
		origin.x = 0;
		origin.y = 0;
		origin.w = TILE_W;
		origin.h = TILE_H;

		Xywh destination{};
		destination.x = _positionInteractDraw_X;
		destination.y = _positionInteractDraw_Y;
		destination.w = TILE_W;
		destination.h = TILE_H;

		if (CanPlant())
			_video->renderGraphic(_idSquare_Red, &origin, &destination);
		else
			_video->renderGraphic(_idSquare_Green, &origin, &destination);
	}
	else if (_activateSquare_Irrigate)
	{
		// Square with world position, real place where to plant
		CalculateSquarePosition(posX, posY, direction);
		// Square drawing position Player/Camera
		CalculateSquarePositionDraw(posXWithCam, posYWithCam, direction);

		Xywh origin{};
		origin.x = 0;
		origin.y = 0;
		origin.w = TILE_W;
		origin.h = TILE_H;

		Xywh destination{};
		destination.x = _positionInteractDraw_X;
		destination.y = _positionInteractDraw_Y;
		destination.w = TILE_W;
		destination.h = TILE_H;

		if (CanIrrigate())
			_video->renderGraphic(_idSquare_Red, &origin, &destination);
		else
			_video->renderGraphic(_idSquare_Blue, &origin, &destination);
	}
}

int Map::NewPositionCamX(int posPlayerX)
{
	return CheckCamLimitX(posPlayerX - (PLAYER_W / 2) - (SIZE_CAMERA_W / 2));
}

int Map::NewPositionCamY(int posPlayerY)
{
	return CheckCamLimitY(posPlayerY - (PLAYER_H / 2) - (SIZE_CAMERA_H / 2));
}

bool Map::CanPlant()
{
	int posX = _posInteract_X / 16;
	int posY = _posInteract_Y / 16;

	if (posX < 0)posX = 0;
	else if (posX > 48)posX = 48;

	if (posY < 0)posY = 0;
	else if (posY > 48)posY = 48;

	bool canPlant = false;
	int sizeLayers = _layers.size();

	for (int i = 0; i < sizeLayers; i++)
	{
		if (_layers.at(i).data[posX][posY] == 1)	// Unirrigated tilled land		
			canPlant = true;

		if (_layers.at(i).data[posX][posY] == 2)	// Irrigated tilled land		
			canPlant = true;
	}

	return canPlant;
}

bool Map::CanIrrigate()
{
	int sizeCrops = _activeCrops.size();
	int posX = _posInteract_X / 16;
	int posY = _posInteract_Y / 16;

	for (int i = 0; i < sizeCrops; i++)
	{
		if (_activeCrops.at(i).posX == posX && _activeCrops.at(i).posY == posY)
		{
			return _activeCrops.at(i).IsIrrigate();
		}
	}

	return true;
}

bool Map::ThereObstacle(int posPlayerX, int posPlayerY)
{
	if (posPlayerX < 0)posPlayerX = 0;
	else if (posPlayerX > 48)posPlayerX = 48;

	if (posPlayerY < 0)posPlayerY = 0;
	else if (posPlayerY > 48)posPlayerY = 48;

	bool thereIsAnObstacle = true;
	int sizeLayers = _layers.size();

	for (int i = 0; i < sizeLayers; i++)
	{
		if (_layers.at(i).data[posPlayerX][posPlayerY] == 122)	// Land		
			thereIsAnObstacle = false;

		if (_layers.at(i).data[posPlayerX][posPlayerY] == 123)	// Land 2		
			thereIsAnObstacle = false;

		if (_layers.at(i).data[posPlayerX][posPlayerY] == 124)	// Land 3		
			thereIsAnObstacle = false;
	}

	return thereIsAnObstacle;
}

bool Map::ThereObstacle_East(int camJugX, int camJugY)
{
	if (camJugX < SIZE_WORLD_W - PLAYER_W)
	{
		if (!ThereObstacle((camJugX + 16) / 16, (camJugY + 0) / 16)) // Upper corner
		{
			if (!ThereObstacle((camJugX + 16) / 16, (camJugY + 23) / 16)) // Lower corner
			{
				return false;
			}
		}
	}

	return true;
}

bool Map::ThereObstacle_West(int camJugX, int camJugY)
{
	if (camJugX > 0)
	{
		if (!ThereObstacle((camJugX - 1) / 16, (camJugY + 0) / 16)) // Upper corner
		{
			if (!ThereObstacle((camJugX - 1) / 16, (camJugY + 23) / 16)) // Lower corner
			{
				return false;
			}
		}
	}
	return true;
}

bool Map::ThereObstacle_North(int camJugX, int camJugY)
{
	if (camJugY > 0)
	{
		if (!ThereObstacle((camJugX + 0) / 16, (camJugY - 1) / 16)) // Upper corner
		{
			if (!ThereObstacle((camJugX + 15) / 16, (camJugY - 1) / 16)) // Lower corner
			{
				return false;
			}
		}
	}
	return true;
}

bool Map::ThereObstacle_South(int camJugX, int camJugY)
{
	if (camJugY < SIZE_WORLD_H - PLAYER_H)
	{
		if (!ThereObstacle((camJugX + 0) / 16, (camJugY + 24) / 16)) // Upper corner
		{
			if (!ThereObstacle((camJugX + 15) / 16, (camJugY + 24) / 16)) // Lower corner
			{
				return false;
			}
		}
	}
	return true;
}

void Map::Carve()
{
	// TODO: check if you can work in this position
	_layers.at(0).data[_posInteract_X / 16][_posInteract_Y / 16] = 1;
}

void Map::Plant(TypeSeeds whichSeed)
{
	if (CanPlant())
	{
		Crop cropTmp;
		switch (whichSeed)
		{
		case GRASS:			cropTmp = _typesCrops.at(GRASS);		break;
		case TURNIP:		cropTmp = _typesCrops.at(TURNIP);		break;
		case POTATO:		cropTmp = _typesCrops.at(POTATO);		break;
		case TOMATO:		cropTmp = _typesCrops.at(TOMATO);		break;
		case WHEAT:			cropTmp = _typesCrops.at(WHEAT);		break;
		case EGGPLANT:		cropTmp = _typesCrops.at(EGGPLANT);		break;
		case PEANUT:		cropTmp = _typesCrops.at(PEANUT);		break;
		case CARROT:		cropTmp = _typesCrops.at(CARROT);		break;
		case BROCCOLI:		cropTmp = _typesCrops.at(BROCCOLI);		break;

		default: break;
		}

		cropTmp.PlantSeed();
		_layers.at(0).data[_posInteract_X / 16][_posInteract_Y / 16] = cropTmp.idTiles.at(0);
		_activeCrops.push_back(cropTmp);
	}
}

void Map::Irrigate()
{
	if (CanIrrigate())
	{
		// PENDENT: irrigate if not
		if (_layers.at(0).data[_posInteract_X / 16][_posInteract_Y / 16] == 1) {
			_layers.at(0).data[_posInteract_X / 16][_posInteract_Y / 16] = 2;
		}
	}

}

void Map::Manure()
{
}

void Map::ModifyData_PlantTomato()
{
	_layers.at(0).data[_posInteract_X / 16][_posInteract_Y / 16] = 53;
}

void Map::ModifyData_Disbudding()
{
	// TODO: change name
	_layers.at(0).data[_posInteract_X / 16][_posInteract_Y / 16] = 122;
}

void Map::SaveMap()
{
	//opening a file in writing mode which is default.
	ofstream file;
	file.open("Resources/Images/Tmx/Granja.tmx");

	// Construct XML on three strings : upper part + modified data + rest	
	string part1, part2, part3;

	// Superior
	part1 += "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
	part1 += "<map version=\"1.5\" tiledversion=\"1.7.1\" orientation=\"orthogonal\" renderorder=\"right-down\" width=\"49\" height=\"49\" tilewidth=\"16\" tileheight=\"16\" infinite=\"0\" nextlayerid=\"7\" nextobjectid=\"1\">\n";
	part1 += "<tileset firstgid=\"1\" source=\"Tileset cultius.tsx\"/>\n";
	part1 += "<tileset firstgid=\"122\" source=\"Tileset obstacles.tsx\"/>\n";
	part1 += "<tileset firstgid=\"158\" source=\"Edifici casa eines.tsx\"/>\n";
	part1 += "<tileset firstgid=\"183\" source=\"Edifici casa.tsx\"/>\n";
	part1 += "<tileset firstgid=\"203\" source=\"Edifici estable gallina.tsx\"/>\n";
	part1 += "<tileset firstgid=\"223\" source=\"Edifici estable vaca.tsx\"/>\n";
	part1 += "<tileset firstgid=\"248\" source=\"Edifici sitja.tsx\"/>\n";
	part1 += "<tileset firstgid=\"278\" source=\"Caseta 1.tsx\"/>\n";
	part1 += "<tileset firstgid=\"284\" source=\"Caseta 2.tsx\"/>\n";
	part1 += "<tileset firstgid=\"290\" source=\"Intern.tsx\"/>\n";
	part1 += "<imagelayer id=\"2\" name=\"Image Layer 1\">\n";
	part1 += "<image source=\"../base.png\" width=\"784\" height=\"784\"/>\n";
	part1 += "</imagelayer>\n";
	part1 += "<layer id=\"1\" name=\"Modificable\" width=\"49\" height=\"49\">\n";
	part1 += "<data encoding=\"csv\">\n";

	// Modified data
	part2 = TransformDataToString();

	// Rest
	part3 += "</data>\n";
	part3 += "</layer>\n";
	part3 += "<layer id=\"4\" name=\"Inamovible\" width=\"49\" height=\"49\">\n";
	part3 += "<data encoding=\"csv\">\n";
	part3 += "125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,146,148,125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,152,154,125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,125,125,125,125,125,125,125,278,279,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,146,148,125,125,\n";
	part3 += "125,0,0,0,125,124,124,124,124,124,125,280,281,0,0,0,0,0,0,0,0,0,248,249,250,251,252,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,152,154,125,125,\n";
	part3 += "125,0,0,0,125,124,124,124,124,124,125,282,283,129,0,0,0,223,224,225,226,227,253,254,255,256,257,203,204,205,206,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,125,183,184,185,186,187,125,0,0,0,0,0,0,228,229,230,231,232,258,259,260,261,262,207,208,209,210,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,125,188,189,190,191,192,125,0,0,0,0,284,285,233,234,235,236,237,263,264,265,266,267,211,212,213,214,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,125,193,194,195,196,197,125,0,0,0,0,286,287,238,239,240,241,242,268,269,270,271,272,215,216,217,218,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,125,198,199,200,201,202,125,0,0,0,129,288,289,243,244,245,246,247,273,274,275,276,277,219,220,221,222,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,124,0,125,125,0,0,0,0,0,0,0,0,124,0,0,0,0,129,0,0,0,124,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,124,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,124,0,0,0,0,0,0,0,0,0,0,0,0,124,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,124,0,0,0,0,0,0,0,0,0,0,0,0,124,0,158,159,160,161,162,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,125,125,125,125,124,124,124,125,125,0,0,0,0,0,0,0,0,0,0,124,0,163,164,165,166,167,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "124,124,124,124,124,124,124,124,124,125,0,0,0,0,0,0,0,0,0,0,124,0,168,169,170,171,172,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "124,124,124,124,124,124,124,124,124,125,0,0,0,0,0,0,0,0,0,0,124,0,173,174,175,176,177,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "124,124,124,124,124,124,124,124,124,125,0,0,0,0,0,0,0,0,0,0,124,0,178,179,180,181,182,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "124,124,124,124,124,124,124,134,135,125,0,0,0,0,0,0,0,0,0,0,124,0,0,0,124,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "124,124,124,124,124,129,124,136,137,125,0,0,0,0,0,0,0,0,0,0,124,124,124,124,124,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,125,125,125,125,125,125,125,125,125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,146,147,148,125,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,149,150,151,125,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,152,153,154,125,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,0,0,0,125,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,125,146,148,125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,0,0,0,0,0,125,152,154,125,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,125,\n";
	part3 += "125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,\n";
	part3 += "123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,\n";
	part3 += "123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,\n";
	part3 += "123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123,123\n";
	part3 += "</data>\n";
	part3 += "</layer>\n";
	part3 += "<layer id=\"7\" name=\"Checkpoints\" width=\"49\" height=\"49\">\n";
	part3 += "<data encoding=\"csv\">\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,291,0,0,0,0,0,0,0,0,0,0,0,292,0,0,0,0,0,0,0,0,293,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "290,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "290,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "290,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,294,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "290,0,0,0,0,0,0,295,295,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "290,0,0,0,0,0,0,295,295,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,\n";
	part3 += "0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0\n";
	part3 += "</data>\n";
	part3 += "</layer>\n";
	part3 += "</map>\n";

	// Write and close
	string resultat = part1 + part2 + part3;
	file << resultat << endl;
	file.close();
}

int Map::CellContent()
{
	int x = _posInteract_X / 16;
	int y = _posInteract_Y / 16;

	if (x < 0)x = 0;
	else if (x > 48)x = 48;

	if (y < 0)y = 0;
	else if (y > 48)y = 48;

	// Layer of logic
	return _layers.at(2).data[x][y];
}

void Map::ResetPositionSquare()
{
	_posInteract_X = 0;
	_posInteract_Y = 0;
}

Map::~Map() {}

#pragma endregion


#pragma region Protected

Map::Map(Video* videoManager, ResourceManager* rscManager)
{
	_width = -1;
	_heigth = -1;

	_tileWidth = -1;
	_tileHeight = -1;

	_camX = 0;
	_camY = 0;

	_video = videoManager;
	_rscManager = rscManager;

	LoadMap();
	LoadExtras();
	LoadCrops();
}

#pragma endregion


#pragma region Private


#pragma region Get info

void Map::LoadMap()
{
	// Open file and check	
	if (_tmx.LoadFile("Resources/Images/Tmx/Granja.tmx") != tinyxml2::XML_SUCCESS)
		cout << "Load TMX file (Granja.tmx) ->" << _tmx.ErrorStr();
	else
		cout << "Load TMX file (Granja.tmx) ->OK" << endl;

	cout << "-------------------------------------------------------------------" << endl;

	GetAttributes();
	GetTilesets();
	GetLayers();
}

void Map::GetAttributes()
{
	// Get and set attributes: <map>	
	_map = _tmx.FirstChildElement("map");
	int widthTempo = -1, heightTempo = -1, tileWidthTempo = -1, tileHeightTempo = -1;

	/// Get attributes of <map>
	_map->QueryAttribute("width", &widthTempo);
	_map->QueryAttribute("height", &heightTempo);
	_map->QueryAttribute("tilewidth", &tileWidthTempo);
	_map->QueryAttribute("tileheight", &tileHeightTempo);

	/// Set attributes of <map>
	_width = widthTempo * tileWidthTempo;
	_heigth = heightTempo * tileHeightTempo;
	_tileWidth = tileWidthTempo;
	_tileHeight = tileHeightTempo;
}

void Map::GetTilesets()
{
	// Get and set the different tilesets: <map> -> <tileset>* n
	tinyxml2::XMLElement* tileset = _map->FirstChildElement("tileset");
	for (tinyxml2::XMLElement* tileset = _map->FirstChildElement("tileset"); tileset != NULL; tileset = tileset->NextSiblingElement("tileset"))
	{
		Tileset tilesetTempo;
		int firstGidTempo = -1;
		const char* sourceTempo;

		/// Get attributes of actual tileset
		tileset->QueryAttribute("firstgid", &firstGidTempo);
		sourceTempo = tileset->Attribute("source");
		tilesetTempo = GetinformationAdditionalTileset(sourceTempo, tilesetTempo);

		/// Set attributes of actual tileset
		tilesetTempo.setFirstgid(firstGidTempo);
		tilesetTempo.setSourceTsx(sourceTempo);

		// Upload the tileset to the resourceManager
		tilesetTempo.setIdRscManager(_rscManager->loadAndGetGraphicID(tilesetTempo.getSourceImage().c_str()));

		/// Add tileset in the vector
		_tilesets.push_back(tilesetTempo);
	}
}

Tileset Map::GetinformationAdditionalTileset(const char* sourceTempo, Tileset tilesetTempo)
{
	// Get the fields: numColumns and image path
	int numColumns = -1;
	int numFiles = -1;
	int tileCount = -1;
	string pathTsx = "Resources/Images/Tmx/";
	string pathImage = "Resources/Images/";
	pathTsx += sourceTempo;

	tinyxml2::XMLDocument _tmxTemp;

	if (_tmxTemp.LoadFile(pathTsx.c_str()) != tinyxml2::XML_SUCCESS)
		cout << "Load TSX file (" << pathTsx.c_str() << ") ->" << _tmxTemp.ErrorStr();
	else
		cout << "Load TSX file (" << pathTsx.c_str() << ") ->OK " << endl;

	cout << "-------------------------------------------------------------------" << endl;

	tinyxml2::XMLElement* tileset = _tmxTemp.FirstChildElement("tileset");
	tileset->QueryAttribute("columns", &numColumns);
	tileset->QueryAttribute("tilecount", &tileCount);
	numFiles = tileCount / numColumns;

	tinyxml2::XMLElement* image = tileset->FirstChildElement("image");
	string pathImageToBeTreated = image->Attribute("source");
	pathImage += pathImageToBeTreated.replace(0, 3, "");

	tilesetTempo.setSourceTsx(pathTsx.c_str());
	tilesetTempo.setSourceImage(pathImage.c_str());
	tilesetTempo.setColumns(numColumns);
	tilesetTempo.setFiles(numFiles);

	return tilesetTempo;
}

void Map::GetLayers()
{
	// Get and set the differents layers: <map> -> <layer>* n
	tinyxml2::XMLElement* layer = _map->FirstChildElement("layer");
	for (tinyxml2::XMLElement* layer = _map->FirstChildElement("layer"); layer != NULL; layer = layer->NextSiblingElement("layer"))
	{
		Layer layerTempo;

		int idTempo = -1;
		int widthTempo = -1;
		int heightTempo = -1;
		const char* nameTempo;
		std::string dirtyData;

		/// Get the different attributes
		layer->QueryAttribute("id", &idTempo);
		layer->QueryAttribute("width", &widthTempo);
		layer->QueryAttribute("height", &heightTempo);
		nameTempo = layer->Attribute("name");

		/// Save them in a Layer
		layerTempo.setId(idTempo);
		layerTempo.setWidth(widthTempo);
		layerTempo.setHeight(heightTempo);
		layerTempo.setName(nameTempo);

		/// Get dirty data
		tinyxml2::XMLElement* data = layer->FirstChildElement("data");
		dirtyData = data->GetText();

		/// Initialize two-dimensional vector "dataContent" (vector of vectors<int>) (width=X, height=Y)
		vector<vector<int>>dataContent;
		int actualWidthLayer = layerTempo.getWidth();
		int actualHeigthLayer = layerTempo.getHeight();
		for (int x = 0; x < actualWidthLayer; x++)
		{
			vector<int>vectorTemp;
			dataContent.push_back(vectorTemp);
		}

		/// Convert "dirtyData" of the current layer and save in a vector(without commas or line breaks)
		int sizeData = dirtyData.size();
		int initPointer = 0;
		int finalPointer = 0;
		vector<int>vectorCleanData;

		for (int i = 0; i < sizeData; i++)
		{
			if (dirtyData[i] == ',')
			{
				finalPointer = i - 1;
				string strTempo = dirtyData.substr(initPointer, finalPointer);
				vectorCleanData.push_back(stoi(strTempo));
				initPointer = i + 1;
			}
			else if (dirtyData[i] == '\n')
			{
				initPointer = i + 1;
			}
			else if (i == sizeData - 2)
			{
				std::string strTempo = dirtyData.substr(initPointer, i);
				vectorCleanData.push_back(stoi(strTempo));
			}
		}

		/// Write the clean data in the vector of vectors<int>"contingutData" by doing a pushback() for 
		/// the whole row X and then increment Y to make a new row of pushbacks().

		int pointerCleanVector = 0;
		for (int y = 0; y < actualHeigthLayer; y++)
		{
			for (int x = 0; x < actualWidthLayer; x++)
			{
				dataContent[x].push_back(vectorCleanData[pointerCleanVector]);
				pointerCleanVector++;
			}
		}

		/// Assign Data in the object Layer
		layerTempo.data = dataContent;

		/// Save each layer in a vector of layers
		_layers.push_back(layerTempo);
	}
}

int Map::SearchTileset(int tileOfDraw)
{
	int whichTileset = -1;
	bool found = false;
	int count = 0;
	int tilesetSize = _tilesets.size();

	if (tileOfDraw > 0)
	{
		while (!found)
		{
			if (count + 1 == tilesetSize) {
				found = true;
				whichTileset = tilesetSize - 1;
			}
			else if (tileOfDraw < _tilesets.at(count + 1).getFirstgid()) {
				found = true;
				whichTileset = count;
			}
			else
				count++;
		}
	}

	return whichTileset;
}

void Map::LoadExtras()
{
	_idSquare_Green = _rscManager->loadAndGetGraphicID("Resources/Images/Green.png");
	_idSquare_Blue = _rscManager->loadAndGetGraphicID("Resources/Images/Blue.png");
	_idSquare_Red = _rscManager->loadAndGetGraphicID("Resources/Images/Red.png");
	_idSquare_Pink = _rscManager->loadAndGetGraphicID("Resources/Images/Pink.png");

	_rscManager->setAlphaGraphic(_idSquare_Green, 255 / 2);
	_rscManager->setAlphaGraphic(_idSquare_Blue, 255 / 2);
	_rscManager->setAlphaGraphic(_idSquare_Red, 255 / 2);
	_rscManager->setAlphaGraphic(_idSquare_Pink, 255 / 2);
}

void Map::LoadCrops()
{
	tinyxml2::XMLDocument fileCultius;

	// Open file and check	
	if (fileCultius.LoadFile("Resources/XMLs/Cultius.xml") != tinyxml2::XML_SUCCESS)
		cout << "Load XML file (Cultius.xml) ->" << fileCultius.ErrorStr();
	else
		cout << "Load XML file (Cultius.xml) ->OK" << endl;

	cout << "-------------------------------------------------------------------" << endl;

	// Get list <crop>
	tinyxml2::XMLElement* crops = fileCultius.FirstChildElement("CULTIUS");

	// Get and set the differents plants: <CULTIUS> -> <PLANTA>* n
	tinyxml2::XMLElement* plants = crops->FirstChildElement("PLANTA");
	for (tinyxml2::XMLElement* plants = crops->FirstChildElement("PLANTA"); plants != NULL; plants = plants->NextSiblingElement("PLANTA"))
	{
		Crop cropTmp;

		/// Obtain information on each camp of the current PLANT
		tinyxml2::XMLElement* nameTmp = plants->FirstChildElement("nom");
		cropTmp.name = nameTmp->GetText();

		tinyxml2::XMLElement* priceTmp = plants->FirstChildElement("preu");
		cropTmp.price = stoi(priceTmp->GetText());

		tinyxml2::XMLElement* daysTmp = plants->FirstChildElement("dies");
		cropTmp.daysToGrow = stoi(daysTmp->GetText());

		tinyxml2::XMLElement* numPhasesTmp = plants->FirstChildElement("numFases");
		cropTmp.numPhases = stoi(numPhasesTmp->GetText());

		////// Processar Fases
		tinyxml2::XMLElement* phasesTmp = plants->FirstChildElement("fases");
		string tmp = phasesTmp->GetText();
		int sizePhases = tmp.size();
		int initPointer = 0;
		int sizeNum = 0;

		for (int i = 0; i < sizePhases + 1; i++)
		{
			if (tmp[i] == ',' || i == sizePhases)
			{
				string strTempo = tmp.substr(initPointer, sizeNum);
				cropTmp.phases.push_back(stoi(strTempo));
				initPointer = i + 1;
				sizeNum = 0;
			}
			else
				sizeNum++;
		}

		////// Process seasons
		tinyxml2::XMLElement* seasonTmp = plants->FirstChildElement("estacio");
		tmp = seasonTmp->GetText();
		int sizeSeason = tmp.size();
		initPointer = 0;
		sizeNum = 0;

		for (int i = 0; i < sizeSeason + 1; i++)
		{
			if (tmp[i] == ',' || i == sizeSeason)
			{
				string strTempo = tmp.substr(initPointer, sizeNum);

				if (strTempo == "Primavera") cropTmp.whenToPlant.push_back(SPRING);
				else if (strTempo == "Estiu") cropTmp.whenToPlant.push_back(SUMMER);
				else if (strTempo == "Tardo") cropTmp.whenToPlant.push_back(AUTUMN);
				else if (strTempo == "Hivern") cropTmp.whenToPlant.push_back(WINTER);

				initPointer = i + 1;
				sizeNum = 0;
			}
			else
				sizeNum++;
		}

		tinyxml2::XMLElement* renewableTmp = plants->FirstChildElement("renovable");
		if (renewableTmp->GetText() == "si") cropTmp.renewable = true;
		else cropTmp.renewable = false;

		tinyxml2::XMLElement* tilemapTmp = plants->FirstChildElement("tilemap");
		cropTmp.nameTilemap = tilemapTmp->GetText();

		////// Process tileNums		
		tinyxml2::XMLElement* tileNumsTmp = plants->FirstChildElement("tilenums");
		tmp = tileNumsTmp->GetText();
		int sizeTileNums = tmp.size();
		initPointer = 0;
		sizeNum = 0;

		for (int i = 0; i < sizeTileNums + 1; i++)
		{
			if (tmp[i] == ',' || i == sizeTileNums)
			{
				string strTempo = tmp.substr(initPointer, sizeNum);
				cropTmp.idTiles.push_back(stoi(strTempo));
				initPointer = i + 1;
				sizeNum = 0;
			}
			else
				sizeNum++;
		}

		// Add the list
		_typesCrops.push_back(cropTmp);
	}
}

#pragma endregion


#pragma region Internal functions

int Map::CheckCamLimitX(int camX)
{
	if (camX < 0)
		return 0;
	else if (camX > SIZE_WORLD_W - SIZE_CAMERA_W)
		return SIZE_WORLD_W - SIZE_CAMERA_W;
	else
		return camX;
}

int Map::CheckCamLimitY(int camY)
{
	if (camY < 0)
		return 0;
	else if (camY > SIZE_WORLD_H - SIZE_CAMERA_H)
		return (SIZE_WORLD_H - SIZE_CAMERA_H);
	else
		return camY;
}

void Map::CalculateSquarePosition(int posX, int posY, Direction direction)
{
	int tmpPosX = posX / 16 * 16;
	int tmpPosY = posY / 16 * 16;

	switch (direction)
	{
	case NORTH:
		_posInteract_X = tmpPosX;
		_posInteract_Y = tmpPosY - 16;
		break;

	case SOUTH:
		_posInteract_X = tmpPosX;
		_posInteract_Y = tmpPosY + 32;
		break;

	case WEST:
		_posInteract_X = tmpPosX - 16;
		_posInteract_Y = tmpPosY + 16;
		break;

	case EAST:
		_posInteract_X = tmpPosX + 16 + 16;
		_posInteract_Y = tmpPosY + 16;
		break;

	default:
		break;
	}
}

void Map::CalculateSquarePositionDraw(int posX, int posY, Direction direction)
{
	int tmpPosX = posX / 16 * 16;
	int tmpPosY = posY / 16 * 16;

	switch (direction)
	{
	case NORTH:
		_positionInteractDraw_X = tmpPosX;
		_positionInteractDraw_Y = tmpPosY - 16;
		break;

	case SOUTH:
		_positionInteractDraw_X = tmpPosX;
		_positionInteractDraw_Y = tmpPosY + 32;
		break;

	case WEST:
		_positionInteractDraw_X = tmpPosX - 16;
		_positionInteractDraw_Y = tmpPosY + 16;
		break;

	case EAST:
		_positionInteractDraw_X = tmpPosX + 16 + 16;
		_positionInteractDraw_Y = tmpPosY + 16;
		break;

	default:
		break;
	}
}

string Map::TransformDataToString()
{
	string resultDATA = "";

	int widthLayer = _layers.at(0).getWidth();
	int heigthLayer = _layers.at(0).getHeight();

	for (int y = 0; y < widthLayer; y++)
	{
		for (int x = 0; x < heigthLayer; x++)
		{
			resultDATA += to_string(_layers.at(0).data[x][y]);
			if (y == widthLayer - 1 && x == heigthLayer - 1)
				resultDATA += "";
			else
				resultDATA += ",";
		}
		resultDATA += "\n";
	}

	return resultDATA;
}

#pragma endregion


#pragma endregion