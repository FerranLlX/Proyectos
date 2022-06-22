#pragma once

#include "../../_Others/Llibreries/tinyxml2.h"
#include "../../Engine/Graphics/ResourceManager.h"
#include "../../Engine/Graphics/Video.h"

#include "Layer.h"
#include "Tileset.h"
#include "../Inventory/VegetalProduct/Cultiu.h"
#include "../Jugador/Direccio.h"

#include<iostream>
#include<fstream>


class Mapa {

public:
	static Mapa* getInstance(Video* video, ResourceManager* rscManager) {
		if (_mapInstance == NULL) _mapInstance = new Mapa(video, rscManager);
		return _mapInstance;
	};
	void setVideoManager(Video* video) { _video = video; }
	void setResourceManager(ResourceManager* rscManager) { _rscManager = rscManager; }

	// Get-set
	int getWidth() { return _width; }
	int getHeight() { return _heigth; }
	int getTileWidth() { return _tileWidth; }
	int getTileHeight() { return _tileHeight; }
	bool getActivarRequadre() { return _activarRequadre; }
	void setActivarRequadre(bool activarRequadre) { _activarRequadre = activarRequadre; }
	bool getActivarRequadre_Regar() { return _activarRequadre_Regar; }
	void setActivarRequadre_Regar(bool activarRequadre_Regar) { _activarRequadre_Regar = activarRequadre_Regar; }
	int getPosSquareX() { return _posicioInteractuar_X; }
	int getPosSquareY() { return _posicioInteractuar_Y; }

	// Funcions
	void renderMap(int camX, int camY);
	void RenderRequadre(int posX, int posY, int posXAmbCam, int posYAmbCam, Direcio direcio);
	int NovaPosicioCamX(int posJugadorX);
	int NovaPosicioCamY(int posJugadorY);
	bool EsPotPlantar();
	bool EsPotRegar();
	bool HiHaObstacle(int posJugadorX, int posJugadorY);
	bool HiHaObstacle_EST(int camJugX, int camJugY);
	bool HiHaObstacle_OEST(int camJugX, int camJugY);
	bool HiHaObstacle_NORD(int camJugX, int camJugY);
	bool HiHaObstacle_SUD(int camJugX, int camJugY);
	void Llaurar();
	void Plantar(TypesSeeds quinaLlavor);
	void Regar();
	void Femtar();
	void ModificarData_PlantarTomata();
	void ModificarData_Desplantar();
	void GuardarMapa();
	int ContigutCela();
	void ResetsPosicioRequadre();

	~Mapa();


protected:
	Mapa(Video* videoManager, ResourceManager* rscManager);


private:
	static Mapa* _mapInstance;

	Video* _video;
	ResourceManager* _rscManager;

	// Info del mapa
	int _width;
	int _heigth;
	int _tileWidth;
	int _tileHeight;
	vector<Layer> _layers;
	vector<Tileset> _tilesets;
	tinyxml2::XMLDocument _tmx;
	tinyxml2::XMLElement* _map;

	// Info cultius
	vector<Cultiu> _tipusCultius;
	vector<Cultiu> _cultiusActius; // Llista de totes les plantes que hi han en el mapa

	// Camera i interacions
	int _cameraX;
	int _cameraY;
	int _idSquare_Green;
	int _idSquare_Blue;
	int _idSquare_Red;
	int _idSquare_Pink;
	bool _activarRequadre;
	bool _activarRequadre_Plantar;
	bool _activarRequadre_Regar;
	bool _activarRequadre_Abonar;
	int _posicioInteractuar_X;
	int _posicioInteractuar_Y;
	int _posicioInteractuarDibuix_X;
	int _posicioInteractuarDibuix_Y;

	// Get info
	void LoadMapa();
	void ObtenirAtributs();
	void ObtenirTilesets();
	Tileset ObtenirInformacioAdicionalTileset(const char* sourceTempo, Tileset tilesetTempo);
	void ObtenirLayers();
	int BuscarTileset(int tileAPintar);
	void LoadExtres();
	void LoadCultius();

	// Funcions internes
	int ComprobarLimitsCamX(int camX);
	int ComprobarLimitsCamY(int camY);
	void CalcularPosicioRequadre(int posX, int posY, Direcio direcio);
	void CalcularPosicioRequadreDibuix(int posX, int posY, Direcio direcio);
	string TransformarDataEnString();
};