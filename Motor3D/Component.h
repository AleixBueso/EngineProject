#ifndef COMPONENT_H
#define COMPONENT_H

#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"


class GameObject;

enum component_type
{
	COMPONENT_TRANSFORM,
	COMPONENT_MESH,
	COMPONENT_MATERIAL
};

class Component
{
public:
	Component();
	Component(component_type Type,  GameObject* from, uint number);
	Component(component_type Type, GameObject* from);
	~Component();

	virtual void Enable();
	virtual void Update(float dt) {};
	virtual void Disable();
	virtual void ComponentEditor() {};
	virtual void SetTransformation(math::float4x4 new_matrix) {};
	virtual math::float4x4 GetLocalTransformationMatrix() { math::float4x4 ret = math::float4x4::identity; return ret; };
	virtual math::float4x4 GetGlobalTransformationMatrix() { math::float4x4 ret = math::float4x4::identity; return ret; };
	virtual void SetGlobalTransformationMatrix(math::float4x4 new_matrix) {};

public:

	component_type type;
	uint id;
	bool enabled = true;
	GameObject* parent;
};
#endif // COMPONENT_H

