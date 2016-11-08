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

	void ComponentEditor();

	void Update(float dt);

public:
	math::Frustum frustum;
};

#endif // CAMERA
