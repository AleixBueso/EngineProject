#ifndef _GOMANAGER_H_
#define _GOMANAGER_H_

#include "Globals.h"
#include "Module.h"
#include "GameObject.h"

class GameObjectManager : public Module
{
public:

	GameObjectManager(Application* app, bool start_enabled = true);
	~GameObjectManager();

	bool Start();
	update_status Update(float dt);

	GameObject* CreateGameObject(GameObject* parent = NULL);
	void Delete(GameObject* GO_to_delete);
	void SetTransformHierarchy(const GameObject* game_object);
	
	public:
	GameObject* root = NULL;
	std::list<GameObject*> all_gameobjects;

};

#endif // _GOMANAGER_H_
