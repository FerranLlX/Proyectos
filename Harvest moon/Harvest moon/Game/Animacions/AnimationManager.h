#pragma once
#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include <SDL_render.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

class TextureManager;

class Animation {

public:
	~Animation();

	void Init();
	void Update(double elapsedSeconds);
	void renderMap(const SDL_Rect* destRect) const;

	void removeAnimation(const char* fitxer);
	Sint32 loadAndGetAnimationID(const char* fitxer, SDL_Renderer* renderer);
	string getAnimationPathByID(Sint32 ID);
	SDL_Texture* getAnimationByID(Sint32 ID);
	void printLoadedAnimations();

	static Animation* getInstance() {
		if (_animationManagerInstance == NULL) _animationManagerInstance = new Animation();
		return _animationManagerInstance;
	};


protected:
	Animation();
	//AnimationManager(std::string path, SDL_Renderer* renderer, int frames, float duration, int frameHeight, int frameWidth);


private:
	TextureManager* _texture;
	int _frames;
	float _duration;
	double _currentTime;
	SDL_Rect _sourceRect;
	int _frameHeight;
	int _frameWidth;

	Sint32 addGraphic(const char* file, SDL_Renderer* renderer);
	void updateFirstFreeSlotGraphic();

	vector<SDL_Texture*> _animationsVector;
	map<string, Sint32> _animationManagerIDMap;
	Uint32 _mFirstFreeSlot;

	static Animation* _animationManagerInstance;	// Singleton instance
};
#endif