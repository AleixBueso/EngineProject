#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(component_type type, GameObject* game_object) : Component(type, game_object)
{
	Position.Set(0, 0, 0);
	Scale.Set(1, 1, 1);
	Rotation.Set(0, 0, 0, 0);
}

ComponentTransform::~ComponentTransform()
{

}