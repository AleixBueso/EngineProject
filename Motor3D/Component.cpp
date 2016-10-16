#include "Component.h"
#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "GameObject.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

Component::Component()
{

}

Component::Component(component_type Type, GameObject* from, uint number) : type(Type), parent(from), id(number)
{
}

Component::Component(component_type Type, GameObject* from) : type(Type), parent(from)
{
	if(Type == COMPONENT_TRANSFORM)
		new ComponentTransform(Type, from);

	if (Type == COMPONENT_MESH)
		new ComponentMesh(Type, from);

	if (Type == COMPONENT_MATERIAL)
		new ComponentMaterial(Type, from);

}

Component::~Component()
{
	
}

void Component::Enable()
{
	enabled = true;
}

void Component::Disable()
{
	enabled = false;
}
