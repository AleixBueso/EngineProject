
#include"GameObject Manager.h"
#include "GameObject.h"
#include "Application.h"
#include "Globals.h"

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

GameObject* GameObjectManager::CreateCamera()
{
	GameObject* tmp = new GameObject(root);
	tmp->CreateComponent(COMPONENT_CAMERA, 0);
	tmp->CreateComponent(COMPONENT_TRANSFORM, 0);
	string name = "Camera " + std::to_string(camera_num);
	tmp->name = name.data();	
	root->AddChild(tmp);
	MainCamera = tmp;

	camera_num++;
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

	if (all_gameobjects.size() > 0)
	{

		if (MainCamera->camera->GetCullingActive())
		{
			list<GameObject*>::const_iterator to_draw_it = MainCamera->camera->GetDrawList()->begin();
			while (to_draw_it != MainCamera->camera->GetDrawList()->end())
			{
				App->renderer3D->DrawMesh((ComponentMesh*)(*to_draw_it)->mesh, (ComponentTransform*)(*to_draw_it)->transform, (ComponentMaterial*)(*to_draw_it)->material);
				to_draw_it++;
			}
		}

		else
		{
			list<GameObject*>::const_iterator it = all_gameobjects.begin();
			while (it != all_gameobjects.end())
			{
				App->renderer3D->DrawMesh((ComponentMesh*)(*it)->mesh, (ComponentTransform*)(*it)->transform, (ComponentMaterial*)(*it)->material);

				it++;
			}
		}

		list<GameObject*>::const_iterator update_it = all_gameobjects.begin();
		while (update_it != all_gameobjects.end())
		{
			(*update_it)->Update(dt);
			update_it++;
		}

		SetTransformHierarchy(root);

		// QuadTree


		if (new_mesh_charged)
		{
			SetQuadTree();
			new_mesh_charged = false;
		}

		if(show_quadtree)
			quadtree.Update();
		

		return update_status::UPDATE_CONTINUE;
	}
}

void GameObjectManager::SetQuadTree()
{
	//Setup QuadTree
	quadtree.Create(AABB(vec(-50, -25, -50), vec(50, 25, 50)));

	list<GameObject*>::const_iterator tree_it = all_gameobjects.begin();
	while (tree_it != all_gameobjects.end())
	{
		quadtree.Add((*tree_it));
		tree_it++;
	}
}

void GameObjectManager::SetTransformHierarchy(const GameObject* game_object)
{
	if (game_object->transform)
	{
		if (root->childs.size())
		{
			if (game_object == *root->childs.begin())
				game_object->transform->SetGlobalTransformationMatrix(game_object->transform->GetLocalTransformationMatrix().Transposed());
		}
	}

	if (game_object->transform)
	{
		if (game_object->parent)
		{
			if (game_object->parent->transform)
				game_object->transform->SetGlobalTransformationMatrix(game_object->parent->transform->GetGlobalTransformationMatrix().Transposed() * game_object->transform->GetLocalTransformationMatrix().Transposed());
		}

		else
			game_object->transform->SetGlobalTransformationMatrix(game_object->transform->GetLocalTransformationMatrix().Transposed());
	}

	if (game_object->childs.size() > 0)
	{
		list<GameObject*>::const_iterator it = game_object->childs.begin();
		while (it != game_object->childs.end())
		{
			SetTransformHierarchy(*it);
			it++;
		}
	}
}