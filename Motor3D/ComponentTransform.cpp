#include "ComponentTransform.h"
#include "GameObject.h"
#include "Imgui\imgui.h"

ComponentTransform::ComponentTransform(component_type type, GameObject* game_object) : Component(type, game_object, 0)
{
	position.Set(0, 0, 0);
	scale.Set(1, 1, 1);
	rotation_degree.Set(0, 0, 0);
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

void ComponentTransform::SetTranslation(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;

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

void ComponentTransform::SetScale(float x, float y, float z)
{
	scale.Set(x, y, z);

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

void ComponentTransform::SetRotation(float x, float y, float z, float w)
{
	rotation.Set(x, y, z, w);

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

float4x4 ComponentTransform::GetLocalTransformationMatrix()
{
	return local_transformation.Transposed();
}

float4x4 ComponentTransform::GetGlobalTransformationMatrix()
{
	return global_transformation.Transposed();
}

void ComponentTransform::SetTransformation()
{
	local_transformation = local_transformation.FromTRS(position, rotation, scale);
}

void ComponentTransform::ComponentEditor()
{
	float3 new_pos = position;
	ImGui::Text("Position");
	ImGui::Text("     X           Y            Z");
	if (ImGui::DragFloat3("pos", new_pos.ptr(), 0.5f))
		SetTranslation(new_pos);

	float3 new_scale = scale;
	ImGui::Text("Scale");
	ImGui::Text("     X           Y            Z");
	if (ImGui::DragFloat3("scl", new_scale.ptr(), 0.5f))
		SetScale(new_scale);

	float3 new_rotation = rotation_degree;
	ImGui::Text("Rotation");
	ImGui::Text("     X           Y            Z");
	if (ImGui::DragFloat3("rot", new_rotation.ptr(), 0.5f), 360, -360)
		SetRotation(new_rotation);
}

void ComponentTransform::Update(float dt)
{

	SetTransformation();
}

void ComponentTransform::SetTransformation(math::float4x4 new_matrix)
{
	local_transformation = new_matrix;
}