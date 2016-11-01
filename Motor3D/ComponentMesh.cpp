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
}

void ComponentMesh::Update(float dt)
{
	local_collider.Enclose((float3*)mesh->vertices, mesh->num_vertices);
	CalculateFinalBB();
}

math::AABB ComponentMesh::GetGlobalBox()
{
	return global_collider;
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
	global_collider = obb.MinimalEnclosingAABB();
}
