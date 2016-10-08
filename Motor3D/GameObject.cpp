#include "GameObject.h"

GameObject::GameObject()
{

}
GameObject::GameObject(GameObject* Parent, string _name) : parent(Parent), name(_name)
{
	
}

GameObject::~GameObject()
{
	components.clear();
}

void GameObject::Update()
{

}

void GameObject::AddComponent(Component component)
{
	components.push_back(&component);
}

Component* GameObject::CreateComponent(component_type type, uint id_num)
{
	Component new_component(type, id_num);
	components.push_back(&new_component);
	return &new_component;
}

bool GameObject::DeleteComponent(Component* ComponentToDelete)
{
	bool ret = true;
	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i] == ComponentToDelete)
		{
			components.erase(components.begin() + i);
		}
	}
	ret = false;
	return ret;
}

Component* GameObject::GetById(uint id)
{
	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->id == id)
			return components[i];
	}

	return NULL;
}

GameObject* GameObject::Duplicate(const GameObject GO_to_duplicate)
{
	return (new GameObject(GO_to_duplicate.parent));
}

void GameObject::SetParent(GameObject* Parent)
{
	parent = Parent;
}

void GameObject::AddChild(GameObject* child)
{
	childs.push_back(child);
}

void GameObject::RemoveChild(GameObject* child)
{
	for (uint i = 0; i < childs.size(); i++)
	{
		if (childs[i] == child)
			childs.erase(childs.begin() + i);
	} 
}