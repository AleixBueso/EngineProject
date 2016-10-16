#include "ComponentMaterial.h"
#include "Imgui\imgui.h"
#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"

ComponentMaterial::ComponentMaterial(component_type type, GameObject* game_object)
{
	this->type = type;
	this->parent = game_object;
}

ComponentMaterial::~ComponentMaterial()
{

}

void ComponentMaterial::ComponentEditor()
{
	ImGui::Text("Id Material: ");	ImGui::SameLine();	ImGui::Text("%d", texture_id);
	ImGui::Text("Tex path: %s", path.c_str());
	ImGui::Image((ImTextureID)texture_id, ImVec2(250, 250));
}

void ComponentMaterial::Update(float dt)
{
	glBindTexture(GL_TEXTURE_2D, texture_id);
}