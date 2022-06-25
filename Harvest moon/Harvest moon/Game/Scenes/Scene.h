#pragma once
#include "../../Engine/Graphics/Video.h"
#include "../../Engine/Graphics/ResourceManager.h"

class Scene {

public:
	Scene();
	// virtual: serves to indicate that the methods can be redefined in the inherited classes.	
	virtual void Init(Video* video, ResourceManager* rscManager);	// Init scene.	
	virtual void ReInit();	// Make resets of all the variables/positions of the scene, if necessary.
	virtual void Render();	// Manage the drawings of the scene
	virtual void Update();	// Manage scene updates

	virtual ~Scene();


protected:
	Video* _videoScene;
	ResourceManager* _rscManagerScene;
};