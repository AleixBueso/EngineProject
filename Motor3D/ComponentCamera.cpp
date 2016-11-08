#include "ComponentCamera.h"

ComponentCamera::ComponentCamera(component_type type, GameObject* game_object)
{
	this->type = type;
	this->parent = game_object;
	frustum.SetPos({ 0, 0, 0 });
}

ComponentCamera::~ComponentCamera()
{

}

void ComponentCamera::Update(float dt)
{

}

void ComponentCamera::ComponentEditor()
{

}
