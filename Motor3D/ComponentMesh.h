#ifndef MESH
#define MESH

#include "Globals.h"
#include "Component.h"
#include "GameObject.h"

struct MyMesh;

class ComponentMesh : public Component
{
public:
	ComponentMesh(component_type type, GameObject* game_object);
	~ComponentMesh();

public:
	MyMesh* mesh = NULL;
	
};

#endif // MESH