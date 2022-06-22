#include "AnimationManager.h"

void Animation::Init()
{
}

void Animation::Update(double elapsedSeconds)
{
}

void Animation::renderMap(const SDL_Rect* destRect) const
{
}

void Animation::removeAnimation(const char* fitxer)
{
}

Sint32 Animation::loadAndGetAnimationID(const char* fitxer, SDL_Renderer* renderer)
{
	return Sint32();
}

string Animation::getAnimationPathByID(Sint32 ID)
{
	return string();
}

SDL_Texture* Animation::getAnimationByID(Sint32 ID)
{
	return nullptr;
}

void Animation::printLoadedAnimations()
{
}

Animation::~Animation()
{
}

Animation::Animation()
{
}

Sint32 Animation::addGraphic(const char* file, SDL_Renderer* renderer)
{
	return Sint32();
}

void Animation::updateFirstFreeSlotGraphic()
{
}
