
#include"GameObject Manager.h"
#include "GameObject.h"
#include "Application.h"

GameObjectManager::GameObjectManager(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

GameObjectManager::~GameObjectManager()
{
	if (root != NULL)
		root = nullptr;

	std::list<GameObject*>::iterator it = all_gameobjects.begin();
	while (it != all_gameobjects.end())
	{
		delete (*it);
		it++;
	}

	all_gameobjects.clear();
}

bool GameObjectManager::Start()
{
	GameObject* root_object = App->gameobject_manager->CreateGameObject();
	root_object->name = "Root GameObject";
	App->gameobject_manager->root = root_object;

	return true;
}

GameObject* GameObjectManager::CreateGameObject(GameObject* parent)
{
	GameObject* tmp = new GameObject(parent);
	
	if (parent == NULL)
		root = tmp;

	else
		parent->AddChild(tmp);

	all_gameobjects.push_back(tmp);

	return tmp;
}

void GameObjectManager::Delete(GameObject* GO_to_delete)
{
	if (GO_to_delete->childs.empty() == false)
	{
		std::list<GameObject*>::iterator it = GO_to_delete->childs.begin();
		while (it != GO_to_delete->childs.end())
		{
			Delete(*it);
			it++;
		}
		GO_to_delete->childs.clear();
	}

	if(GO_to_delete->childs.empty())
		delete GO_to_delete;
}

update_status GameObjectManager::Update(float dt)
{

	list<GameObject*>::const_iterator it = all_gameobjects.begin();
	while (it != all_gameobjects.end())
	{
		App->renderer3D->DrawMesh((ComponentMesh*)(*it)->mesh, (ComponentTransform*)(*it)->transform, (ComponentMaterial*)(*it)->material);
		(*it)->Update(dt);
		it++;
	}
	return update_status::UPDATE_CONTINUE;
}