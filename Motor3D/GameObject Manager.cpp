
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
	GameObject* tmp = new GameObject(parent);
	if (parent == NULL)
		root = tmp;
	return tmp;
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
		list<GameObject*>::const_iterator it = GO_to_delete->childs.begin();
		while (it != GO_to_delete->childs.end())
		{
			delete *it;
		}

		tmp->childs.clear();
	}

	delete GO_to_delete;
}