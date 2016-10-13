#pragma once
#ifndef MATERIAL
#define MATERIAL

#include "Globals.h"
#include "Component.h"
#include "GameObject.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial(component_type type, GameObject* game_object);
	~ComponentMaterial();

	void ComponentEditor();

public:
	uint texture_id = 0;
};

#endif // MATERIAL

