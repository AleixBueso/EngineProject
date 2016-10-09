
#include"GameObject Manager.h"
#include "GameObject.h"

GameObjectManager::GameObjectManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

GameObjectManager::~GameObjectManager()
{
	if(root != NULL)
		delete root;
}

GameObject* GameObjectManager::CreateGameObject(GameObject* parent)
{
	return new GameObject(parent);
}

void GameObjectManager::Delete(GameObject* GO_to_delete)
{
	GameObject* tmp = GO_to_delete;

	//Find last child
	while (tmp->childs.empty())
		tmp = *tmp->childs.begin();

	//Delete children to top
	while (tmp != GO_to_delete)
	{
		tmp = tmp->parent;

		//erase childs
		for (uint i = 0; i < tmp->childs.size(); i++)
		{
			delete tmp->childs[i];
		}

		tmp->childs.clear();
	}

	delete GO_to_delete;
}