#pragma once
#ifndef CAMERA
#define CAMERA

#include "Globals.h"
#include "Component.h"
#include "GameObject.h"
#include <vector>
#include "MathGeoLib\include\MathGeoLib.h"

class ComponentCamera : public Component
{
public:
	ComponentCamera(component_type type, GameObject* game_object);
	~ComponentCamera();

	const float GetNearPlane() const;
	const float GetFarPlane() const;
	const float3 GetPos() const;
	bool GetCullingActive();

	void SetNearPlane(float value);
	void SetFarPlane(float value);
	void SetHFov(float value);
	void SetVFov(float value);
	const std::list<GameObject*>* GetDrawList() const;

	void ComponentEditor();

	bool CheckIntersection(const math::AABB & refBox) const;
	void DrawFrustum();
	void PreUpdate(float dt);
	void Update(float dt);

	//void OnSave(Config& config) const;
	//void OnLoad(Config * config);

public:
	math::Frustum frustum;
	bool CullingActive;

private:
	float near_plane;
	float far_plane;
	float horizontal_fov;
	float vertical_fov;

	std::list<GameObject*> game_objects_draw;
};

#endif // CAMERA
