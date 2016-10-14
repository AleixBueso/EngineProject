#pragma once
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

GameObject::GameObject() : name("Empty GameObject")
{

}

GameObject::GameObject(GameObject* Parent, string Name) : parent(Parent), name(Name)
{
	transform = CreateComponent(component_type::COMPONENT_TRANSFORM, 0);
	material = CreateComponent(component_type::COMPONENT_MATERIAL, 0);
	mesh = CreateComponent(component_type::COMPONENT_MESH, 0);
}

GameObject::~GameObject()
{
	components.clear();
}

void GameObject::Update(float dt)
{

	vector<Component*>::iterator it = components.begin();
	while (it != components.end())
	{
		(*it)->Update(dt);
		it++;
	}

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

	if (type == COMPONENT_TRANSFORM)
	{
		transform = new ComponentTransform(type, this);
		components.push_back(transform);
		return transform;
	}

	if (type == COMPONENT_MATERIAL)
	{
		material = new ComponentMaterial(type, this);
		components.push_back(material);
		return material;
	}

	if (type == COMPONENT_MESH)
	{
		mesh = new ComponentMesh(type, this);
		components.push_back(mesh);
		return mesh;
	}

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