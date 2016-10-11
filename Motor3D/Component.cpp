#include "Component.h"
#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "GameObject.h"

Component::Component()
{

}

Component::Component(component_type Type, uint number) : type(Type), id(number)
{
	switch (type)
	{

	case(COMPONENT_TRANSFORM):
		break;

	case(COMPONENT_MESH) :
		break;

	case(COMPONENT_MATERIAL) :
		break;
			
	default:
		break;
	}
}

Component::Component(component_type Type, GameObject* from) : type(Type), parent(from)
{
	switch (type)
	{

	case(COMPONENT_TRANSFORM) :
		break;

	case(COMPONENT_MESH) :
		break;

	case(COMPONENT_MATERIAL) :
		break;

	default:
		break;
	}
}

Component::~Component()
{
}

void Component::Enable()
{
	enabled = true;
}

void Component::Update()
{
	if (enabled)
	{

	}

}

void Component::Disable()
{
	enabled = false;
}
