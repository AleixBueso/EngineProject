#include "ComponentMesh.h"

ComponentMesh::ComponentMesh(GameObject* game_object)
{
	parent = game_object;
}

ComponentMesh::ComponentMesh(component_type type, GameObject* game_object)
{
	this->type = type;
	parent = game_object;
}

ComponentMesh::~ComponentMesh()
{

}