#include "Application.h"
#include "ComponentMesh.h"
#include "Imgui\imgui.h"
#include "ModuleModelLoader.h"

struct MyMesh;

ComponentMesh::ComponentMesh(GameObject* game_object)
{
	parent = game_object;
	local_collider.SetNegativeInfinity();
	global_collider.SetNegativeInfinity();
}

ComponentMesh::ComponentMesh(component_type type, GameObject* game_object)
{
	this->type = type;
	parent = game_object;
	local_collider.SetNegativeInfinity();
	global_collider.SetNegativeInfinity();
}

void ComponentMesh::Update(float dt)
{
	CalculateFinalBB();
}

math::AABB ComponentMesh::GetGlobalBox()
{
	return global_collider;
}

math::AABB ComponentMesh::GetLocalBox()
{
	return local_collider;
}

ComponentMesh::~ComponentMesh()
{
	delete mesh;
}

void ComponentMesh::ComponentEditor()
{
	if (mesh != NULL)
	{
		ImGui::Text("Num vertices: %i", mesh->num_vertices);
		ImGui::Text("Num indices: %i", mesh->num_indices);
		ImGui::Text("Num normlas: %i", mesh->normals);
		ImGui::Text("Num texture coords: %i", mesh->num_texture_coords);
	}
}

void ComponentMesh::CalculateFinalBB()
{
	OBB obb = local_collider.Transform(parent->transform->GetGlobalTransformationMatrix());

	//Transformation
	global_collider = obb.MinimalEnclosingAABB();
}
