#pragma once
#include "Globals.h"
#include "Module.h"
#include "GameObject.h"

class GameObject;

class GameObjectManager : public Module
{
public:

	GameObjectManager(Application* app, bool start_enabled = true);
	~GameObjectManager();

	GameObject* CreateGameObject(GameObject* parent);
	void Delete(GameObject* GO_to_delete);
	
private:
	GameObject* root = NULL;

};
