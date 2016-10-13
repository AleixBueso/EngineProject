#ifndef MESH
#define MESH

#include "Globals.h"
#include "Component.h"
#include "GameObject.h"

struct MyMesh;

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* game_object);
	ComponentMesh(component_type type, GameObject* game_object);
	~ComponentMesh();

	void ComponentEditor();

public:
	MyMesh* mesh = NULL;
	
};

#endif // MESH