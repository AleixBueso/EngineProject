#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(component_type type, GameObject* game_object) : Component(type, game_object)
{
	position.Set(0, 0, 0);
	scale.Set(1, 1, 1);
	rotation.Set(0, 0, 0, 0);
}

ComponentTransform::~ComponentTransform()
{

}