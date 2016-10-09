#ifndef TRANSFORM
#define TRANSFORM


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
	 
	float3 Position;
	float3 Scale;
	Quat Rotation;
	 
};

#endif // !TRANSFORM