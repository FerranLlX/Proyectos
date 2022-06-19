#include "Mapa.h"

Mapa* Mapa::_mapInstance = NULL;


#pragma region Public

void Mapa::renderMap(int camX, int camY)
{
	_cameraX = camX;
	_cameraY = camY;

	int iniciCamX = _cameraX / _tileWidth;
	int iniciCamY = _cameraY / _tileHeight;
	int finalCamX = (MIDA_CAMERA_W / _tileWidth) + iniciCamX;
	int finalCamY = (MIDA_CAMERA_H / _tileHeight) + iniciCamY;

	int numLayers = _layers.size();
	for (int i = 0; i < numLayers - 1; i++) // -1 pq l'últim nivell es de logica (accessos/portes/contenidors)
	{
		// Obtenir data del layer actual
		vector<vector<int>>layerData = _layers.at(i).data;

		int posCamX = 0;
		int posCamY = 0;

		for (int y = iniciCamY; y < finalCamY; y++)
		{
			for (int x = iniciCamX; x < finalCamX; x++)
			{
				// Obtenir tile
				int tileAPintar = layerData[x][y];

				if (tileAPintar != 0)
				{
					// Buscar tileset que tingui el tile a pintar					
					int quinTilsetDelVector = BuscarTileset(tileAPintar);
					if (quinTilsetDelVector != -1)
					{
						Tileset tilesetATreballar = _tilesets.at(quinTilsetDelVector);

						// Buscar id del rscManager
						int idTileset = tilesetATreballar.getIdRscManager();

						// Comprobar el tileAPintar segons el tileset
						tileAPintar = tileAPintar - tilesetATreballar.getFirstgid() + 1;

						// Mida i posicio del tiles del tileset
						Xywh SrcR{};
						Xywh DestR{};

						int posYTile = -1;
						int posXTile = -1;

						int contador = 1;
						bool trobat = false;
						for (int y = 0; y < tilesetATreballar.getFiles(); y++)
						{
							for (int x = 0; x < tilesetATreballar.getColumnes(); x++)
							{
								if (contador == tileAPintar)
								{
									posXTile = x;
									posYTile = y;
									trobat = true;
									break;
								}
								else
									contador++;
							}
							if (trobat)
								break;
						}

						// Multiplicar pel tamany de tile
						posYTile *= _tileHeight;
						posXTile *= _tileWidth;

						// Assignar RECTs SRC-DEST
						SrcR.x = posXTile;
						SrcR.y = posYTile;
						SrcR.w = _tileWidth;
						SrcR.h = _tileHeight;

						DestR.x = x * _tileWidth;
						DestR.y = y * _tileHeight;
						DestR.w = _tileWidth;
						DestR.h = _tileHeight;

						// Suavitzar la camera pq no faci salts de 16 en 16
						DestR.x = DestR.x - _cameraX;
						DestR.y = DestR.y - _cameraY;

						// Pintar tile(del tileset) al render						
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

void Mapa::RenderRequadre(int posX, int posY, int posXAmbCam, int posYAmbCam, Direcio direcio)
{
	if (_activarRequadre)
	{
		// Requadre amb posicio del mon, lloc real on plantar
		CalcularPosicioRequadre(posX, posY, direcio);
		// Requadre dibuix posicio Jugador/Camera
		CalcularPosicioRequadreDibuix(posXAmbCam, posYAmbCam, direcio);

		Xywh origen{};
		origen.x = 0;
		origen.y = 0;
		origen.w = TILE_W;
		origen.h = TILE_H;

		Xywh desti{};
		desti.x = _posicioInteractuarDibuix_X;
		desti.y = _posicioInteractuarDibuix_Y;
		desti.w = TILE_W;
		desti.h = TILE_H;

		if (EsPotPlantar())
			_video->renderGraphic(_idSquare_Red, &origen, &desti);
		else
			_video->renderGraphic(_idSquare_Green, &origen, &desti);
	}
	else if (_activarRequadre_Regar)
	{
		// Requadre amb posicio del mon, lloc real on plantar
		CalcularPosicioRequadre(posX, posY, direcio);
		// Requadre dibuix posicio Jugador/Camera
		CalcularPosicioRequadreDibuix(posXAmbCam, posYAmbCam, direcio);

		Xywh origen{};
		origen.x = 0;
		origen.y = 0;
		origen.w = TILE_W;
		origen.h = TILE_H;

		Xywh desti{};
		desti.x = _posicioInteractuarDibuix_X;
		desti.y = _posicioInteractuarDibuix_Y;
		desti.w = TILE_W;
		desti.h = TILE_H;

		if (EsPotRegar())
			_video->renderGraphic(_idSquare_Red, &origen, &desti);
		else
			_video->renderGraphic(_idSquare_Blue, &origen, &desti);
	}
}

int Mapa::NovaPosicioCamX(int posJugadorX)
{
	return ComprobarLimitsCamX(posJugadorX - (JUGADOR_W / 2) - (MIDA_CAMERA_W / 2));
}

int Mapa::NovaPosicioCamY(int posJugadorY)
{
	return ComprobarLimitsCamY(posJugadorY - (JUGADOR_H / 2) - (MIDA_CAMERA_H / 2));
}

bool Mapa::EsPotPlantar()
{
	int posX = _posicioInteractuar_X / 16;
	int posY = _posicioInteractuar_Y / 16;

	if (posX < 0)posX = 0;
	else if (posX > 48)posX = 48;

	if (posY < 0)posY = 0;
	else if (posY > 48)posY = 48;

	bool pucPlantar = false;
	int sizeLayers = _layers.size();

	for (int i = 0; i < sizeLayers; i++)
	{
		if (_layers.at(i).data[posX][posY] == 1)	// Terra llaurada sense regar		
			pucPlantar = true;

		if (_layers.at(i).data[posX][posY] == 2)	// Terra llaurada regada		
			pucPlantar = true;
	}

	return pucPlantar;
}

bool Mapa::EsPotRegar()
{
	int sizeCultius = _cultiusActius.size();
	int posX = _posicioInteractuar_X / 16;
	int posY = _posicioInteractuar_Y / 16;

	for (int i = 0; i < sizeCultius; i++)
	{
		if (_cultiusActius.at(i).posX == posX && _cultiusActius.at(i).posY == posY)
		{
			return _cultiusActius.at(i).EstaRegat();
		}
	}

	return true;
}

bool Mapa::HiHaObstacle(int posJugadorX, int posJugadorY)
{
	if (posJugadorX < 0)posJugadorX = 0;
	else if (posJugadorX > 48)posJugadorX = 48;

	if (posJugadorY < 0)posJugadorY = 0;
	else if (posJugadorY > 48)posJugadorY = 48;

	bool hihaobstacle = true;
	int sizeLayers = _layers.size();

	for (int i = 0; i < sizeLayers; i++)
	{
		if (_layers.at(i).data[posJugadorX][posJugadorY] == 122)	// Terra		
			hihaobstacle = false;

		if (_layers.at(i).data[posJugadorX][posJugadorY] == 123)	// Terra 2		
			hihaobstacle = false;

		if (_layers.at(i).data[posJugadorX][posJugadorY] == 124)	// Terra 3		
			hihaobstacle = false;
	}

	return hihaobstacle;
}

bool Mapa::HiHaObstacle_EST(int camJugX, int camJugY)
{
	if (camJugX < TAMANY_WORLD_W - JUGADOR_W)
	{
		if (!HiHaObstacle((camJugX + 16) / 16, (camJugY + 0) / 16)) // Cantonada superior
		{
			if (!HiHaObstacle((camJugX + 16) / 16, (camJugY + 23) / 16)) // Cantonada inferior
			{
				return false;
			}
		}
	}

	return true;
}

bool Mapa::HiHaObstacle_OEST(int camJugX, int camJugY)
{
	if (camJugX > 0)
	{
		if (!HiHaObstacle((camJugX - 1) / 16, (camJugY + 0) / 16)) // Cantonada superior
		{
			if (!HiHaObstacle((camJugX - 1) / 16, (camJugY + 23) / 16)) // Cantonada inferior
			{
				return false;
			}
		}
	}
	return true;
}

bool Mapa::HiHaObstacle_NORD(int camJugX, int camJugY)
{
	if (camJugY > 0)
	{
		if (!HiHaObstacle((camJugX + 0) / 16, (camJugY - 1) / 16)) // Cantonada superior
		{
			if (!HiHaObstacle((camJugX + 15) / 16, (camJugY - 1) / 16)) // Cantonada inferior
			{
				return false;
			}
		}
	}
	return true;
}

bool Mapa::HiHaObstacle_SUD(int camJugX, int camJugY)
{
	if (camJugY < TAMANY_WORLD_H - JUGADOR_H)
	{
		if (!HiHaObstacle((camJugX + 0) / 16, (camJugY + 24) / 16)) // Cantonada superior
		{
			if (!HiHaObstacle((camJugX + 15) / 16, (camJugY + 24) / 16)) // Cantonada inferior
			{
				return false;
			}
		}
	}
	return true;
}

void Mapa::Llaurar()
{
	// TODO: comprobar si pot llaura en aquesta posicio
	_layers.at(0).data[_posicioInteractuar_X / 16][_posicioInteractuar_Y / 16] = 1;
}

void Mapa::Plantar(Llavors quinaLlavor)
{
	if (EsPotPlantar())
	{
		Cultiu cultiuTmp;
		switch (quinaLlavor)
		{
		case HERBA:			cultiuTmp = _tipusCultius.at(HERBA);		break;
		case NAP:			cultiuTmp = _tipusCultius.at(NAP);			break;
		case PATATA:		cultiuTmp = _tipusCultius.at(PATATA);		break;
		case TOMATA:		cultiuTmp = _tipusCultius.at(TOMATA);		break;
		case BLAT:			cultiuTmp = _tipusCultius.at(BLAT);			break;
		case ALBERGINIA:	cultiuTmp = _tipusCultius.at(ALBERGINIA);	break;
		case CACAUET:		cultiuTmp = _tipusCultius.at(CACAUET);		break;
		case CARROTA:		cultiuTmp = _tipusCultius.at(CARROTA);		break;
		case BROQUIL:		cultiuTmp = _tipusCultius.at(BROQUIL);		break;

		default: break;
		}

		cultiuTmp.PlantaLlavor();
		_layers.at(0).data[_posicioInteractuar_X / 16][_posicioInteractuar_Y / 16] = cultiuTmp.idTiles.at(0);
		_cultiusActius.push_back(cultiuTmp);
	}
}

void Mapa::Regar()
{
	if (EsPotRegar())
	{
		// PENDENT: regar si no esta regat
		if (_layers.at(0).data[_posicioInteractuar_X / 16][_posicioInteractuar_Y / 16] == 1) {
			_layers.at(0).data[_posicioInteractuar_X / 16][_posicioInteractuar_Y / 16] = 2;
		}
	}

}

void Mapa::Femtar()
{
	// PENDENT: implementar
}

void Mapa::ModificarData_PlantarTomata()
{
	// PENDENT: asdf
	_layers.at(0).data[_posicioInteractuar_X / 16][_posicioInteractuar_Y / 16] = 53;
}

void Mapa::ModificarData_Desplantar()
{
	// TODO: canviar nom
	_layers.at(0).data[_posicioInteractuar_X / 16][_posicioInteractuar_Y / 16] = 122;
}

void Mapa::GuardarMapa()
{
	//opening a file in writing mode which is default.
	ofstream file;
	file.open("Resources/Images/Tmx/Granja.tmx");

	// Construir XML en tres strings: la part superior + data modificada + la resta
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

	// Data modificada
	part2 = TransformarDataEnString();

	// La resta
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

	// Escriure i tancar
	string resultat = part1 + part2 + part3;
	file << resultat << endl;
	file.close();
}

int Mapa::ContigutCela()
{
	int x = _posicioInteractuar_X / 16;
	int y = _posicioInteractuar_Y / 16;

	if (x < 0)x = 0;
	else if (x > 48)x = 48;

	if (y < 0)y = 0;
	else if (y > 48)y = 48;

	// Layer de logica
	return _layers.at(2).data[x][y];
}

void Mapa::ResetsPosicioRequadre()
{
	_posicioInteractuar_X = 0;
	_posicioInteractuar_Y = 0;
}

Mapa::~Mapa() {}

#pragma endregion


#pragma region Protected

Mapa::Mapa(Video* videoManager, ResourceManager* rscManager)
{
	_width = -1;
	_heigth = -1;

	_tileWidth = -1;
	_tileHeight = -1;

	_cameraX = 0;
	_cameraY = 0;

	_video = videoManager;
	_rscManager = rscManager;

	LoadMapa();
	LoadExtres();
	LoadCultius();
}

#pragma endregion


#pragma region Private


#pragma region Get info

void Mapa::LoadMapa()
{
	// Obrir fitxer i comprobar	
	if (_tmx.LoadFile("Resources/Images/Tmx/Granja.tmx") != tinyxml2::XML_SUCCESS)
		cout << "Load TMX file (Granja.tmx) ->" << _tmx.ErrorStr();
	else
		cout << "Load TMX file (Granja.tmx) ->OK" << endl;

	cout << "-------------------------------------------------------------------" << endl;

	ObtenirAtributs();
	ObtenirTilesets();
	ObtenirLayers();
}

void Mapa::ObtenirAtributs()
{
	// Obtenir i setejar atributs: <map>	
	_map = _tmx.FirstChildElement("map");
	int widthTempo = -1, heightTempo = -1, tileWidthTempo = -1, tileHeightTempo = -1;

	/// Obtenir atributs de <map>
	_map->QueryAttribute("width", &widthTempo);
	_map->QueryAttribute("height", &heightTempo);
	_map->QueryAttribute("tilewidth", &tileWidthTempo);
	_map->QueryAttribute("tileheight", &tileHeightTempo);

	/// Setejar atributs de <map>
	_width = widthTempo * tileWidthTempo;
	_heigth = heightTempo * tileHeightTempo;
	_tileWidth = tileWidthTempo;
	_tileHeight = tileHeightTempo;
}

void Mapa::ObtenirTilesets()
{
	// Obtenir i setejar els diferents tilesets: <map> -> <tileset>* n
	tinyxml2::XMLElement* tileset = _map->FirstChildElement("tileset");
	for (tinyxml2::XMLElement* tileset = _map->FirstChildElement("tileset"); tileset != NULL; tileset = tileset->NextSiblingElement("tileset"))
	{
		Tileset tilesetTempo;
		int firstGidTempo = -1;
		const char* sourceTempo;

		/// Obtenir atributs del tileset actual
		tileset->QueryAttribute("firstgid", &firstGidTempo);
		sourceTempo = tileset->Attribute("source");
		tilesetTempo = ObtenirInformacioAdicionalTileset(sourceTempo, tilesetTempo);

		/// Setejar atributs del tileset actual
		tilesetTempo.setFirstgid(firstGidTempo);
		tilesetTempo.setSourceTsx(sourceTempo);

		// Carregar el tileset al resourceManager
		tilesetTempo.setIdRscManager(_rscManager->loadAndGetGraphicID(tilesetTempo.getSourceImage().c_str()));

		/// Afegir tileset en el vector de tilesets
		_tilesets.push_back(tilesetTempo);
	}
}

Tileset Mapa::ObtenirInformacioAdicionalTileset(const char* sourceTempo, Tileset tilesetTempo)
{
	// Obtenir els camps: numColumnes i ruta de l'imatge
	int numColumnes = -1;
	int numFiles = -1;
	int tilecount = -1;
	string rutaTsx = "Resources/Images/Tmx/";
	string rutaImage = "Resources/Images/";
	rutaTsx += sourceTempo;

	tinyxml2::XMLDocument _tmxTemporal;

	if (_tmxTemporal.LoadFile(rutaTsx.c_str()) != tinyxml2::XML_SUCCESS)
		cout << "Load TSX file (" << rutaTsx.c_str() << ") ->" << _tmxTemporal.ErrorStr();
	else
		cout << "Load TSX file (" << rutaTsx.c_str() << ") ->OK " << endl;

	cout << "-------------------------------------------------------------------" << endl;

	tinyxml2::XMLElement* tileset = _tmxTemporal.FirstChildElement("tileset");
	tileset->QueryAttribute("columns", &numColumnes);
	tileset->QueryAttribute("tilecount", &tilecount);
	numFiles = tilecount / numColumnes;

	tinyxml2::XMLElement* image = tileset->FirstChildElement("image");
	string rutaImatgeTractar = image->Attribute("source");
	rutaImage += rutaImatgeTractar.replace(0, 3, "");

	tilesetTempo.setSourceTsx(rutaTsx.c_str());
	tilesetTempo.setSourceImage(rutaImage.c_str());
	tilesetTempo.setColumnes(numColumnes);
	tilesetTempo.setFiles(numFiles);

	return tilesetTempo;
}

void Mapa::ObtenirLayers()
{
	// Obtenir i setejar els diferents layers: <map> -> <layer>* n
	tinyxml2::XMLElement* layer = _map->FirstChildElement("layer");
	for (tinyxml2::XMLElement* layer = _map->FirstChildElement("layer"); layer != NULL; layer = layer->NextSiblingElement("layer"))
	{
		Layer layerTempo;

		int idTempo = -1;
		int widthTempo = -1;
		int heightTempo = -1;
		const char* nameTempo;
		std::string dataEnBrut;

		/// Obtenir els diferents atributs
		layer->QueryAttribute("id", &idTempo);
		layer->QueryAttribute("width", &widthTempo);
		layer->QueryAttribute("height", &heightTempo);
		nameTempo = layer->Attribute("name");

		/// Guardar-los en l'objecte Layer
		layerTempo.setId(idTempo);
		layerTempo.setWidth(widthTempo);
		layerTempo.setHeight(heightTempo);
		layerTempo.setName(nameTempo);

		/// Obtenir data en brut
		tinyxml2::XMLElement* data = layer->FirstChildElement("data");
		dataEnBrut = data->GetText();

		/// Inicialitzar vector bidimensional "contingutData" (vector de vectors<int>) (width=X, height=Y)
		vector<vector<int>>contingutData;
		int widthLayerActual = layerTempo.getWidth();
		int heigthLayerActual = layerTempo.getHeight();
		for (int x = 0; x < widthLayerActual; x++)
		{
			vector<int>vectorTemporal;
			contingutData.push_back(vectorTemporal);
		}

		/// Convertir "dataEnBrut" del layer actual i guardar en un vector tractat (sense comes ni salts de linies)
		int sizeData = dataEnBrut.size();
		int punterInici = 0;
		int punterFinal = 0;
		vector<int>vectorDataTractat;

		for (int i = 0; i < sizeData; i++)
		{
			if (dataEnBrut[i] == ',')
			{
				punterFinal = i - 1;
				string strTempo = dataEnBrut.substr(punterInici, punterFinal);
				vectorDataTractat.push_back(stoi(strTempo));
				punterInici = i + 1;
			}
			else if (dataEnBrut[i] == '\n')
			{
				punterInici = i + 1;
			}
			else if (i == sizeData - 2)
			{
				std::string strTempo = dataEnBrut.substr(punterInici, i);
				vectorDataTractat.push_back(stoi(strTempo));
			}
		}

		/// Escriure les dades tractades en el vector de vectors<int>"contingutData" fent un pushback() per tota la fila X i llavors incrementa Y per fer una nova fila de pushbacks()
		int punterVectorTractat = 0;
		for (int y = 0; y < heigthLayerActual; y++)
		{
			for (int x = 0; x < widthLayerActual; x++)
			{
				contingutData[x].push_back(vectorDataTractat[punterVectorTractat]);
				punterVectorTractat++;
			}
		}

		/// Assignar Data en l'objecte Layer
		layerTempo.data = contingutData;

		/// Guardar cada layer en un vector de layers
		_layers.push_back(layerTempo);
	}
}

int Mapa::BuscarTileset(int tileAPintar)
{
	int quinTileset = -1;
	bool trobat = false;
	int count = 0;
	int tilesetSize = _tilesets.size();

	if (tileAPintar > 0)
	{
		while (!trobat)
		{
			if (count + 1 == tilesetSize) {
				trobat = true;
				quinTileset = tilesetSize - 1;
			}
			else if (tileAPintar < _tilesets.at(count + 1).getFirstgid()) {
				trobat = true;
				quinTileset = count;
			}
			else
				count++;
		}
	}

	return quinTileset;
}

void Mapa::LoadExtres()
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

void Mapa::LoadCultius()
{
	tinyxml2::XMLDocument fileCultius;

	// Obrir fitxer i comprobar	
	if (fileCultius.LoadFile("Resources/XMLs/Cultius.xml") != tinyxml2::XML_SUCCESS)
		cout << "Load XML file (Cultius.xml) ->" << fileCultius.ErrorStr();
	else
		cout << "Load XML file (Cultius.xml) ->OK" << endl;

	cout << "-------------------------------------------------------------------" << endl;

	// Obtenir llista <cultius>
	tinyxml2::XMLElement* cultius = fileCultius.FirstChildElement("CULTIUS");

	// Obtenir i setejar les diferents plantes: <CULTIUS> -> <PLANTA>* n
	tinyxml2::XMLElement* plantes = cultius->FirstChildElement("PLANTA");
	for (tinyxml2::XMLElement* plantes = cultius->FirstChildElement("PLANTA"); plantes != NULL; plantes = plantes->NextSiblingElement("PLANTA"))
	{
		Cultiu cultiuTmp;

		/// Obtenir informacio de cada camp de la PLANTA actual
		tinyxml2::XMLElement* nomTmp = plantes->FirstChildElement("nom");
		cultiuTmp.nom = nomTmp->GetText();

		tinyxml2::XMLElement* preuTmp = plantes->FirstChildElement("preu");
		cultiuTmp.preu = stoi(preuTmp->GetText());

		tinyxml2::XMLElement* diesTmp = plantes->FirstChildElement("dies");
		cultiuTmp.diesPerCreixer = stoi(diesTmp->GetText());

		tinyxml2::XMLElement* numFasesTmp = plantes->FirstChildElement("numFases");
		cultiuTmp.numFases = stoi(numFasesTmp->GetText());

		////// Processar Fases
		tinyxml2::XMLElement* fasesTmp = plantes->FirstChildElement("fases");
		string tmp = fasesTmp->GetText();
		int sizeFases = tmp.size();
		int punterInici = 0;
		int llargadaNum = 0;

		for (int i = 0; i < sizeFases + 1; i++)
		{
			if (tmp[i] == ',' || i == sizeFases)
			{
				string strTempo = tmp.substr(punterInici, llargadaNum);
				cultiuTmp.fases.push_back(stoi(strTempo));
				punterInici = i + 1;
				llargadaNum = 0;
			}
			else
				llargadaNum++;
		}

		////// Processar estacions
		tinyxml2::XMLElement* estacioTmp = plantes->FirstChildElement("estacio");
		tmp = estacioTmp->GetText();
		int sizeEstacions = tmp.size();
		punterInici = 0;
		llargadaNum = 0;

		for (int i = 0; i < sizeEstacions + 1; i++)
		{
			if (tmp[i] == ',' || i == sizeEstacions)
			{
				string strTempo = tmp.substr(punterInici, llargadaNum);

				if (strTempo == "Primavera") cultiuTmp.quanPlantar.push_back(PRIMAVERA);
				else if (strTempo == "Estiu") cultiuTmp.quanPlantar.push_back(ESTIU);
				else if (strTempo == "Tardo") cultiuTmp.quanPlantar.push_back(TARDO);
				else if (strTempo == "Hivern") cultiuTmp.quanPlantar.push_back(HIVERN);

				punterInici = i + 1;
				llargadaNum = 0;
			}
			else
				llargadaNum++;
		}

		tinyxml2::XMLElement* renovableTmp = plantes->FirstChildElement("renovable");
		if (renovableTmp->GetText() == "si") cultiuTmp.renovable = true;
		else cultiuTmp.renovable = false;

		tinyxml2::XMLElement* tilemapTmp = plantes->FirstChildElement("tilemap");
		cultiuTmp.nomTilemap = tilemapTmp->GetText();

		////// Processar tileNums		
		tinyxml2::XMLElement* tileNumsTmp = plantes->FirstChildElement("tilenums");
		tmp = tileNumsTmp->GetText();
		int sizeTileNums = tmp.size();
		punterInici = 0;
		llargadaNum = 0;

		for (int i = 0; i < sizeTileNums + 1; i++)
		{
			if (tmp[i] == ',' || i == sizeTileNums)
			{
				string strTempo = tmp.substr(punterInici, llargadaNum);
				cultiuTmp.idTiles.push_back(stoi(strTempo));
				punterInici = i + 1;
				llargadaNum = 0;
			}
			else
				llargadaNum++;
		}

		// Afegir a la llista
		_tipusCultius.push_back(cultiuTmp);
	}
}

#pragma endregion


#pragma region Funcions internes

int Mapa::ComprobarLimitsCamX(int camX)
{
	if (camX < 0)
		return 0;
	else if (camX > TAMANY_WORLD_W - MIDA_CAMERA_W)
		return TAMANY_WORLD_W - MIDA_CAMERA_W;
	else
		return camX;
}

int Mapa::ComprobarLimitsCamY(int camY)
{
	if (camY < 0)
		return 0;
	else if (camY > TAMANY_WORLD_H - MIDA_CAMERA_H)
		return (TAMANY_WORLD_H - MIDA_CAMERA_H);
	else
		return camY;
}

void Mapa::CalcularPosicioRequadre(int posX, int posY, Direcio direcio)
{
	int tmpPosX = posX / 16 * 16;
	int tmpPosY = posY / 16 * 16;

	switch (direcio)
	{
	case NORD:
		_posicioInteractuar_X = tmpPosX;
		_posicioInteractuar_Y = tmpPosY - 16;
		break;

	case SUD:
		_posicioInteractuar_X = tmpPosX;
		_posicioInteractuar_Y = tmpPosY + 32;
		break;

	case OEST:
		_posicioInteractuar_X = tmpPosX - 16;
		_posicioInteractuar_Y = tmpPosY + 16;
		break;

	case EST:
		_posicioInteractuar_X = tmpPosX + 16 + 16;
		_posicioInteractuar_Y = tmpPosY + 16;
		break;

	default:
		break;
	}
}

void Mapa::CalcularPosicioRequadreDibuix(int posX, int posY, Direcio direcio)
{
	int tmpPosX = posX / 16 * 16;
	int tmpPosY = posY / 16 * 16;

	switch (direcio)
	{
	case NORD:
		_posicioInteractuarDibuix_X = tmpPosX;
		_posicioInteractuarDibuix_Y = tmpPosY - 16;
		break;

	case SUD:
		_posicioInteractuarDibuix_X = tmpPosX;
		_posicioInteractuarDibuix_Y = tmpPosY + 32;
		break;

	case OEST:
		_posicioInteractuarDibuix_X = tmpPosX - 16;
		_posicioInteractuarDibuix_Y = tmpPosY + 16;
		break;

	case EST:
		_posicioInteractuarDibuix_X = tmpPosX + 16 + 16;
		_posicioInteractuarDibuix_Y = tmpPosY + 16;
		break;

	default:
		break;
	}
}

string Mapa::TransformarDataEnString()
{
	string resultatDATA = "";

	int widthLayer = _layers.at(0).getWidth();
	int heigthLayer = _layers.at(0).getHeight();

	for (int y = 0; y < widthLayer; y++)
	{
		for (int x = 0; x < heigthLayer; x++)
		{
			resultatDATA += to_string(_layers.at(0).data[x][y]);
			if (y == widthLayer - 1 && x == heigthLayer - 1)
				resultatDATA += "";
			else
				resultatDATA += ",";
		}
		resultatDATA += "\n";
	}

	return resultatDATA;
}

#pragma endregion


#pragma endregion