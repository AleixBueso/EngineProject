#pragma once
#ifndef CAMERA
#define CAMERA

#include "Globals.h"
#include "Component.h"
#include "GameObject.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include <list>
#include "p2Point.h"

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

enum Direction
{
	GO_RIGHT,
	GO_LEFT,
	GO_UP,
	GO_DOWN
};

class ComponentCamera : public Component
{
public:
	ComponentCamera(component_type type, GameObject* game_object);
	~ComponentCamera();

	void Update(float dt);
	bool CleanUp();

	void Look(const vec &Position, const vec &Reference, bool RotateAroundReference = false);
	void LookAt(const vec &Spot);
	void Move(const vec &Movement);
	void Move(Direction d, float speed);
	float* GetViewMatrix();

	void Rotate(float x, float y);

	//Transform a 3D point to a point of the screen
	void From3Dto2D(vec point, int& x, int& y);

public:
	math::Frustum frustum;
	vec X, Y, Z, Position, Reference;

private:
	void CalculateViewMatrix();
	float4x4 ViewMatrix, ViewMatrixInverse;
};

#endif // CAMERA

