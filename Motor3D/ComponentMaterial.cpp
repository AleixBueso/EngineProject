#include "ComponentMaterial.h"
#include "Imgui\imgui.h"

ComponentMaterial::ComponentMaterial(component_type type, GameObject* game_object) : Component(type, game_object)
{

}

ComponentMaterial::~ComponentMaterial()
{

}

void ComponentMaterial::ComponentEditor()
{
	ImGui::Text("Id Material: ");
	ImGui::SameLine();
	ImGui::Text("%d", texture_id);
	ImGui::Image((ImTextureID)texture_id, ImVec2(250, 250));
}