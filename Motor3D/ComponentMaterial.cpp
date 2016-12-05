#include "ComponentMaterial.h"
#include "Imgui\imgui.h"
#include "Glew\include\glew.h"
#include "Application.h"
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
	static int texture_type_editor;
	ImGui::Text("Id Material: ");	ImGui::SameLine();	ImGui::Text("%d", texture_id);
	ImGui::Text("Tex path: %s", path.c_str());
	ImGui::Combo("Texture Type", &texture_type_editor, "Solid\0Alpha\0Blend\0\0");

	if(texture_type == ALPHA)
		ImGui::SliderFloat("Alpha Test", &alpha_test, 0, 0.95);

	ImGui::Image((ImTextureID)texture_id, ImVec2(250, 250));

	if (texture_type_editor == 0)
		texture_type = SOLID;

	if (texture_type_editor == 1)
		texture_type = ALPHA;

	if (texture_type_editor == 2)
		texture_type = BLEND;
}

void ComponentMaterial::Update(float dt)
{
	glBindTexture(GL_TEXTURE_2D, texture_id);
}