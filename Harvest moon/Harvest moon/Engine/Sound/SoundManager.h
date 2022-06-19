#pragma once
#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <SDL_mixer.h>

class SoundManager
{

public:
	~SoundManager();

	void removeChunck(const char* fitxer);
	Sint32 loadAndGetChunckID(const char* fitxer);
	std::string getChunckPathByID(Sint32 ID);
	Mix_Chunk* getChunckByID(Sint32 ID);
	void printChuncksLoaded();	// Pinta el path dels Chunck carregats

	static SoundManager* getInstance() {
		if (_soundManagerInstance == NULL) _soundManagerInstance = new SoundManager();
		return _soundManagerInstance;
	};//! Gets Singleton instance	


protected:
	SoundManager();


private:
	Sint32 addChunk(const char* file);
	void updateFirstFreeSlotChunk();

	std::vector<Mix_Chunk*> _chunksVector; // Vector that stores Surfaces. Useful in render methods and sequential access
	std::map<std::string, Sint32> _chunkIDMap; // Map that stores ID. Links strings to ID, Useful for check if graphic is has been loaded previously

	Uint32 _firstFreeSlotChunk; // First free slot in the mSoundVector

	static SoundManager* _soundManagerInstance; // Singleton instance
};

#endif