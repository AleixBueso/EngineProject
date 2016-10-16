#include "ComponentMesh.h"
#include "Imgui\imgui.h"
#include "ModuleModelLoader.h"

struct MyMesh;

ComponentMesh::ComponentMesh(GameObject* game_object)
{
	parent = game_object;
}

ComponentMesh::ComponentMesh(component_type type, GameObject* game_object)
{
	this->type = type;
	parent = game_object;
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
		ImGui::Text("Num UVs: %i", mesh->num_UVs);
	}
}