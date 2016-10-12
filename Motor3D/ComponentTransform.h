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

	void SetTranslation(float3 newpos);
	float3 GetTranslation();

	void SetScale(float3 new_scale);
	float3 GetScale();

	void SetRotation(float3 rot);
	void SetRotation(Quat rot);
	float3 GetRotation();

	float4x4 GetTransformationMatrix();

	void SetTransformation();
	void ComponentEditor();

	void Update(uint dt);

public:
	 
	float3 position;
	float3 scale;
	Quat rotation;
	float3 rotation_degree;

	float4x4 transformation = float4x4::identity;
	float4x4 final_transformation = float4x4::identity;
	 
};

#endif // !TRANSFORM_H