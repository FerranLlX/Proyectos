#pragma once
#include <string>

using namespace std;

class Tileset
{
public:
	Tileset();
	~Tileset();

	int getFirstgid() { return _firstgid; }
	void setFirstgid(int firstGid) { _firstgid = firstGid; }

	int getColumnes() { return _numeroColumnes; }
	void setColumnes(int columnes) { _numeroColumnes = columnes; }

	int getFiles() { return _numeroFiles; }
	void setFiles(int files) { _numeroFiles = files; }

	string getSourceTsx() { return _sourceTsx; }
	void setSourceTsx(const char* sourceTsx) { _sourceTsx = sourceTsx; }

	string getSourceImage() { return _sourceImage; }
	void setSourceImage(const char* sourceImage) { _sourceImage = sourceImage; }

	int getIdRscManager() { return _idRscMngJugador; }
	void setIdRscManager(int id) { _idRscMngJugador = id; }

	void loadTileset();


private:
	int _firstgid; // firstgrid numero amb que comença cada tileset(ex: tileset1 = 1-15, tileset2 = 16-50)
	int _numeroColumnes; // segons la mida del tileset
	int _numeroFiles; // segons la mida del tileset
	string _sourceTsx; // fitxer grafic associat TSX
	string _sourceImage; // ruta de l'imatge
	int _idRscMngJugador; // id del grafic al nostre ResourceManager
};

