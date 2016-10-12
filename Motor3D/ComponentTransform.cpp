#include "ComponentTransform.h"
#include "GameObject.h"
#include "Imgui\imgui.h"

ComponentTransform::ComponentTransform(component_type type, GameObject* game_object) : Component(type, game_object, 0)
{
	position.Set(0, 0, 0);
	scale.Set(1, 1, 1);
	rotation.Set(0, 0, 0, 0);
}

ComponentTransform::~ComponentTransform()
{

}

void ComponentTransform::SetTranslation(float3 new_pos)
{
	position.x = new_pos.x;
	position.y = new_pos.y;
	position.z = new_pos.z;

	SetTransformation();
}

float3 ComponentTransform::GetTranslation()
{
	return position;
}

void ComponentTransform::SetScale(float3 new_scale)
{
	scale.Set(new_scale.x, new_scale.y, new_scale.z);

	SetTransformation();

}
float3 ComponentTransform::GetScale()
{
	return scale;
}

void ComponentTransform::SetRotation(float3 rot)
{
	rotation_degree = rot;

	float3 rotation_rad;

	rotation_rad.x = DegToRad(rotation_degree.x);
	rotation_rad.y = DegToRad(rotation_degree.y);
	rotation_rad.z = DegToRad(rotation_degree.z);

	rotation = Quat::FromEulerXYZ(rotation_rad.x, rotation_rad.y, rotation_rad.z);

	SetTransformation();
}

void ComponentTransform::SetRotation(Quat rot)
{
	rotation.Set(rot.x, rot.y, rot.z, rot.w);

	SetTransformation();
}

float3 ComponentTransform::GetRotation()
{
	float3 tmp = rotation.ToEulerXYZ();

	RadToDeg(tmp.x);
	RadToDeg(tmp.y);
	RadToDeg(tmp.z);

	return tmp;
}

float4x4 ComponentTransform::GetTransformationMatrix()
{
	return final_transformation.Transposed();
}

void ComponentTransform::SetTransformation()
{
	transformation = transformation.FromTRS(position, rotation, scale);
}

void ComponentTransform::ComponentEditor()
{
	ImGui::Text("Position");
	ImGui::DragFloat("X", &position.x); ImGui::SameLine(); ImGui::DragFloat("Y", &position.y); ImGui::SameLine(); ImGui::DragFloat("Z", &position.z);
	ImGui::Text("Scale");
	ImGui::DragFloat("X", &scale.x); ImGui::SameLine(); ImGui::DragFloat("Y", &scale.x); ImGui::SameLine(); ImGui::DragFloat("Z", &scale.x);
	ImGui::Text("Rotation");
}

void ComponentTransform::Update(uint dt)
{

}