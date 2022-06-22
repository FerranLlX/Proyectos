#include "ResourceManager.h"

#pragma region PUBLIC

ResourceManager* ResourceManager::_resourceManagerInstance = NULL;


Sint32 ResourceManager::loadAndGetGraphicID(const char* file)
{
	// Check if exist, or add
	if (_resourceManagerIDMap.count(file) > 0)
		return _resourceManagerIDMap.find(file)->second;
	else
		return addGraphic(file);
}

string ResourceManager::getGraphicPathByID(Sint32 ID)
{
	map<string, Sint32>::iterator it;

	for (it = _resourceManagerIDMap.begin(); it != _resourceManagerIDMap.end(); it++)
	{
		if (it->second == ID)
			return it->first;
	}

	return "";
}

SDL_Texture* ResourceManager::getGraphicByID(Sint32 id)
{
	return _graphicsVector[id];
}

void ResourceManager::getGraphicSize(Sint32 img, int& width, int& height)
{
	SDL_Texture* imatgeAAnalitzar = getGraphicByID(img);
	SDL_Point size;
	SDL_QueryTexture(imatgeAAnalitzar, NULL, NULL, &size.x, &size.y);
	width = size.x;
	height = size.y;
}

Uint16 ResourceManager::getGraphicWidth(Sint32 img)
{
	int width;
	int height;
	getGraphicSize(img, width, height);

	return width;
}

Uint16 ResourceManager::getGraphicHeight(Sint32 img)
{
	int width;
	int height;
	getGraphicSize(img, width, height);

	return height;
}

void ResourceManager::setAlphaGraphic(Sint32 ID, Uint8 alpha_value)
{
	SDL_Texture* imgInfo = getGraphicByID(ID);
	SDL_SetTextureAlphaMod(imgInfo, alpha_value);
}

void ResourceManager::printLoadedGraphics()
{
	for (map<string, Sint32>::iterator it = _resourceManagerIDMap.begin(); it != _resourceManagerIDMap.end(); ++it)
		cout << it->first << " => " << it->second << '\n';

	cout << "-------------------------------------------------------------------" << endl;

	int size = _graphicsVector.size();
	for (int i = 0; i < size; i++)
	{
		cout << _graphicsVector.at(i) << endl;
	}
}

void ResourceManager::removeGraphic(const char* file)
{
	// It is not possible to make a delete but to equalize to NULL, so that the positions of the pointers do not move.
	SDL_DestroyTexture(_graphicsVector.at(_resourceManagerIDMap.find(file)->second));
	_graphicsVector.at(_resourceManagerIDMap.find(file)->second) = nullptr;

	// When inserting a new Texture use the _firstFreeSlotChunk variable to know where to place it.
	updateFirstFreeSlotGraphic();
}


ResourceManager::~ResourceManager() {

	// Release all textures

	//SDL_DestroyTexture(bmpTex);
	//SDL_DestroyRenderer(renderer);
	//SDL_DestroyWindow(win);
	//SDL_Quit();
}


#pragma endregion



#pragma region PROTECTED

ResourceManager::ResourceManager()
{
	_mFirstFreeSlot = -1;
	_render = nullptr;
}

#pragma endregion



#pragma region PRIVATE

Sint32 ResourceManager::addGraphic(const char* file)
{
	string nomFitxer = file;

	SDL_Surface* surfaceTemp = IMG_Load(file);
	if (surfaceTemp == NULL) cout << "Load Imatge file (" + nomFitxer + ") -> ERROR - " << IMG_GetError() << endl;
	else cout << "Load Imatge file (" + nomFitxer + ") -> OK" << endl;

	SDL_Texture* textureTmp = SDL_CreateTextureFromSurface(_render, surfaceTemp);
	if (textureTmp == NULL) cout << "Crear textura (" + nomFitxer + ") -> ERROR - " << SDL_GetError() << endl;
	else cout << "Crear textura (" + nomFitxer + ") -> OK" << endl;

	cout << "-------------------------------------------------------------------" << endl;

	// Activate the texture in Blend mode to allow transperencies.
	SDL_SetTextureBlendMode(textureTmp, SDL_BLENDMODE_BLEND);

	SDL_FreeSurface(surfaceTemp);

	updateFirstFreeSlotGraphic();

	if (_mFirstFreeSlot == -1) {
		_graphicsVector.push_back(textureTmp);
		int size = _graphicsVector.size();
		map<string, Sint32>::iterator it = _resourceManagerIDMap.begin();
		_resourceManagerIDMap.insert(it, pair<string, Sint32>(file, size - 1));
		return size - 1;
	}
	else {
		_graphicsVector.insert(_graphicsVector.begin() + _mFirstFreeSlot, textureTmp);
		map<string, Sint32>::iterator it = _resourceManagerIDMap.begin();
		_resourceManagerIDMap.insert(it, pair<string, Sint32>(file, _mFirstFreeSlot));
		return _mFirstFreeSlot;
	}

	SDL_DestroyTexture(textureTmp);
}

void ResourceManager::updateFirstFreeSlotGraphic()
{
	int size = _graphicsVector.size();
	int i = 0;
	bool trobat = false;

	if (size == 0) {
		_mFirstFreeSlot = -1;
	}
	else if (size > 0)
	{
		while (!trobat && i <= size - 1)
		{
			if (_graphicsVector.at(i) == NULL) {
				_mFirstFreeSlot = i;
				trobat = true;
			}
			i++;
		}

		if (!trobat)
		{
			// -1 means that you will have to create a new Slot to add a new resource
			_mFirstFreeSlot = -1;
		}
	}
	else {
		_mFirstFreeSlot = -1;
	}
}

#pragma endregion