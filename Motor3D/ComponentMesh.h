#ifndef MESH
#define MESH

#include "Globals.h"
#include "Component.h"
#include "GameObject.h"
#include "MathGeoLib\include\MathGeoLib.h"

struct MyMesh;

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* game_object);
	ComponentMesh(component_type type, GameObject* game_object);
	void Update(float dt);
	const math::AABB GetGlobalBox();
	const math::AABB GetLocalBox();
	~ComponentMesh();

	void CalculateFinalBB();

	void ComponentEditor();

public:

	math::AABB local_collider;
	math::AABB global_collider;
	MyMesh* mesh = NULL;
	
};

#endif // MESH