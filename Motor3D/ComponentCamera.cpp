#include "ComponentCamera.h"
#include "Application.h"
#include "Module.h"

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
	static float3 points[8];
	frustum.GetCornerPoints(points);
	App->renderer3D->DrawBox(points, Red);
}

void ComponentCamera::ComponentEditor()
{

}
