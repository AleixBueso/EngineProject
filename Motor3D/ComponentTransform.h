#ifndef TRANSFORM_H
#define TRANSFORM_H


#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Component.h"

#include "GameObject.h"

using namespace math;

class ComponentTransform : public Component
{
public:
	ComponentTransform(component_type type, GameObject* game_object);
	~ComponentTransform();

public:
	 
	float3 position;
	float3 scale;
	Quat rotation;
	 
};

#endif // !TRANSFORM_H