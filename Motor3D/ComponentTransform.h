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
	void SetTranslation(float x, float y, float z);
	float3 GetTranslation();

	void SetScale(float3 new_scale);
	void SetScale(float x, float y, float z);
	float3 GetScale();

	void SetRotation(float3 rot);
	void SetRotation(Quat rot);
	void SetRotation(float x, float y, float z, float w);
	float3 GetRotation();

	float4x4 GetLocalTransformationMatrix();
	float4x4 GetGlobalTransformationMatrix();
	void SetGlobalTransformationMatrix(math::float4x4 new_matrix);
	void SetTransformation(math::float4x4 new_matrix);

	void SetTransformation();
	void ComponentEditor();


	void Update(float dt);

public:
	 
	float3 position;
	float3 scale;
	Quat rotation;
	float3 rotation_degree;

	math::float3 rotation_euler = math::float3::zero;

	float4x4 final_transformation = float4x4::identity;

	float4x4 local_transformation = float4x4::identity;
	float4x4 global_transformation = float4x4::identity;
	 
};

#endif // !TRANSFORM_H