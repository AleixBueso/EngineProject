#pragma once
#include "Globals.h"
#include "Module.h"
#include "GameObject.h"

class GameObjectManager : public Module
{
public:

	GameObjectManager(Application* app, bool start_enabled = true);
	~GameObjectManager();

	GameObject* CreateGameObject(GameObject* parent = NULL);
	void Delete(GameObject* GO_to_delete);
	
	public:
	GameObject* root = NULL;

};
