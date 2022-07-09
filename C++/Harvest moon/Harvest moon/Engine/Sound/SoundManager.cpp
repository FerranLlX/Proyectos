#include "SoundManager.h"

#pragma region PUBLIC

SoundManager* SoundManager::_soundManagerInstance = NULL;

SoundManager::~SoundManager()
{
}

void SoundManager::removeChunck(const char* fitxer)
{
	Mix_FreeChunk(_chunksVector.at(_chunkIDMap.find(fitxer)->second));
	_chunksVector.at(_chunkIDMap.find(fitxer)->second) = nullptr;
	updateFirstFreeSlotChunk();
}

Sint32 SoundManager::loadAndGetChunckID(const char* fitxer)
{
	// Comprova si exiteix algun, en cas contrari l'afegeix
	if (_chunkIDMap.count(fitxer) > 0)
		return _chunkIDMap.find(fitxer)->second;
	else
		return addChunk(fitxer);
}

std::string SoundManager::getChunckPathByID(Sint32 ID)
{
	std::map<std::string, Sint32>::iterator it;

	for (it = _chunkIDMap.begin(); it != _chunkIDMap.end(); it++)
	{
		if (it->second == ID)
		{
			return it->first;
		}
	}

	return "";
}

Mix_Chunk* SoundManager::getChunckByID(Sint32 ID)
{
	std::map<std::string, Sint32>::iterator it;
	int posicio = 0;

	for (it = _chunkIDMap.begin(); it != _chunkIDMap.end(); it++)
	{
		if (it->second == ID) {
			break;
		}
		posicio++;
	}

	return _chunksVector[posicio];
}

void SoundManager::printChuncksLoaded()
{
	for (std::map<std::string, Sint32>::iterator it = _chunkIDMap.begin(); it != _chunkIDMap.end(); ++it)
		std::cout << it->first << " => " << it->second << '\n';
}

#pragma endregion



#pragma region PROTECTED

SoundManager::SoundManager()
{
	_firstFreeSlotChunk = 0;

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	int result = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	// Check
	if (result != 0)
		std::cout << "Failed to open audio: " << Mix_GetError() << std::endl;

	Mix_AllocateChannels(128);
}

#pragma endregion



#pragma region PRIVATE

Sint32 SoundManager::addChunk(const char* file)
{
	Mix_Chunk* chunkTempo = Mix_LoadWAV(file);

	// Check loads
	if (!chunkTempo)
		std::cout << "Failed to load music or sound: " << Mix_GetError() << std::endl;

	updateFirstFreeSlotChunk();

	if (_firstFreeSlotChunk == 0) {
		_chunksVector.push_back(chunkTempo);

		int size = _chunksVector.size();
		std::map<std::string, Sint32>::iterator it = _chunkIDMap.begin();
		_chunkIDMap.insert(it, std::pair<std::string, Sint32>(file, size - 1));
		return size - 1;
	}
	else {
		_chunksVector.insert(_chunksVector.begin() + _firstFreeSlotChunk, chunkTempo);

		std::map<std::string, Sint32>::iterator it = _chunkIDMap.begin();
		_chunkIDMap.insert(it, std::pair<std::string, Sint32>(file, _firstFreeSlotChunk));
		return _firstFreeSlotChunk;
	}
}

void SoundManager::updateFirstFreeSlotChunk()
{
	int size = _chunksVector.size();

	if (size > 0)
	{
		for (int i = 0; i < size; i++)
		{
			if (_chunksVector.at(i) == NULL)
				_firstFreeSlotChunk = i;
		}
	}
	else
		_firstFreeSlotChunk = 0;
}

#pragma endregion