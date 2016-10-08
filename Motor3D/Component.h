#pragma once
#include "Globals.h"

enum component_type
{
	COMPONENT_TRANSFORM,
	COMPONENT_MESH,
	COMPONENT_MATERIAL
};

class Component
{
public:
	Component(component_type Type, uint number);
	~Component();

	virtual void Enable();
	virtual void Update();
	virtual void Disable();

public:

	component_type type;
	uint id;
	bool enabled = true;

};


