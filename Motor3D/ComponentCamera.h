#pragma once
#ifndef CAMERA
#define CAMERA

#include "Globals.h"
#include "Component.h"
#include "GameObject.h"
#include "MathGeoLib\include\MathGeoLib.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera(component_type type, GameObject* game_object);
	~ComponentCamera();

	const float GetNearPlane() const;
	const float GetFarPlane() const;
	const float3 GetPos() const;

	void SetNearPlane(float value);
	void SetFarPlane(float value);
	void SetHFov(float value);
	void SetVFov(float value);

	void ComponentEditor();

	void DrawFrustum();
	void Update(float dt);

public:
	math::Frustum frustum;

private:
	float near_plane;
	float far_plane;
	float horizontal_fov;
	float vertical_fov;
};

#endif // CAMERA
