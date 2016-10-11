#ifndef COMPONENT_H
#define COMPONENT_H

#include "Globals.h"


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
	Component(component_type Type, uint number);
	Component(component_type Type, GameObject* from);
	~Component();

	virtual void Enable();
	virtual void Update(float dt);
	virtual void Disable();

public:

	component_type type;
	uint id;
	bool enabled = true;
	GameObject* parent;
};
#endif // COMPONENT_H

