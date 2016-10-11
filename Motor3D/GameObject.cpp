#pragma once
#include "GameObject.h"

GameObject::GameObject() : name("Empty GameObject")
{
	CreateComponent(COMPONENT_TRANSFORM, 0);
	CreateComponent(COMPONENT_MATERIAL, 1);
	CreateComponent(COMPONENT_MESH, 2);
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

void GameObject::AddComponent(Component* component)
{
	if (component->type == COMPONENT_TRANSFORM)
		transform = component;
	if (component->type == COMPONENT_MATERIAL)
		material = component;
	if (component->type == COMPONENT_MESH)
		mesh = component;

	components.push_back(component);
}

Component* GameObject::CreateComponent(component_type type, uint id_num)
{
	Component* new_component = new Component(type, id_num);

	if (new_component->type == COMPONENT_TRANSFORM)
		transform = new_component;
	if (new_component->type == COMPONENT_MATERIAL)
		material = new_component;
	if (new_component->type == COMPONENT_MESH)
		mesh = new_component;

	components.push_back(new_component);
	return new_component;
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
	list<GameObject*>::const_iterator it = childs.begin();
	while (it != childs.end())
	{
		if (*it == child)
			childs.erase(it);
		it++;
	}
}