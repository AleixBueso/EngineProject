#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Globals.h"
#include "Component.h"
#include "Module.h"
#include <stdlib.h>
#include <list>
#include <vector>

class GameObject 
{
public:

	GameObject();
	GameObject(GameObject* Parent = NULL, std::string Name = "Empty GameObject");
	~GameObject();

	void Update(float dt);

	void AddComponent(Component* component);
	Component* CreateComponent(component_type type, uint id_num);
	bool DeleteComponent(Component* ComponentToDelete);
	Component* GetById(uint id);

	GameObject* Duplicate(const GameObject GO_to_duplicate);
	void SetParent(GameObject* parent);

	void AddChild(GameObject* child);
	void RemoveChild(GameObject* child);



public:
	
	Component* transform = nullptr;
	Component* material = nullptr;
	Component* mesh = nullptr;
	Component* camera = nullptr;

	bool is_static = false;

	UINT32 UUID = 0;

	std::vector<Component*> components;
	GameObject* parent;
	std::list<GameObject*> childs;
	
	std::string name;
};

#endif // GAMEOBJECT_H