#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Globals.h"
#include "Component.h"
#include "Module.h"
#include <stdlib.h>
#include <list>
#include <vector>


using namespace std;

class GameObject
{
public:

	GameObject();
	GameObject(GameObject* Parent = NULL, string _name = "Empty GameObject");
	~GameObject();

	void Update();

	void AddComponent(Component* component);
	Component* CreateComponent(component_type type, uint id_num);
	bool DeleteComponent(Component* ComponentToDelete);
	Component* GetById(uint id);

	GameObject* Duplicate(const GameObject GO_to_duplicate);
	void SetParent(GameObject* parent);

	void AddChild(GameObject* child);
	void RemoveChild(GameObject* child);



public:
	
	Component* transform;
	Component* material;
	Component* mesh;

	vector<Component*> components;
	GameObject* parent;
	list<GameObject*> childs;
	
	string name;
};

#endif // GAMEOBJECT_H