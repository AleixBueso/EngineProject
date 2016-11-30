#include "Globals.h"
#include "ComponentCamera.h"
#include "Application.h"
#include "Module.h"

#include "SDL\include\SDL_opengl.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Imgui\imgui.h"

ComponentCamera::ComponentCamera(component_type type, GameObject* game_object)
{
	this->type = type;
	this->parent = game_object;
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	near_plane = 5;
	far_plane = 50;
	horizontal_fov = 60;
	vertical_fov = 60;
	frustum.SetPos({ 0, 0, 0 });
	frustum.SetFront({ 0, 0, 1 });
	frustum.SetUp({ 0, 1, 0 });
	frustum.SetViewPlaneDistances(5, 50);
	frustum.SetPerspective(DEGTORAD * horizontal_fov, DEGTORAD * horizontal_fov);
	CullingActive = false;
}

ComponentCamera::~ComponentCamera()
{

}

void ComponentCamera::PreUpdate(float dt)
{

}

void ComponentCamera::Update(float dt)
{
	game_objects_draw.clear();

	list<GameObject*>::const_iterator it = App->gameobject_manager->all_gameobjects.begin();
	while (it != App->gameobject_manager->all_gameobjects.end())
	{
		if ((*it)->mesh)
		{
			if (CheckIntersection((*it)->mesh->GetGlobalBox()))
			{
				game_objects_draw.push_back(*it);
			}
		}				
		it++;
	}

	DrawFrustum();


	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
	{
		GetPick();
	}
	
}

GameObject* ComponentCamera::GetPick()
{
	float width = (float)App->window->screen_surface->w;
	float height = (float)App->window->screen_surface->h;

	int mouse_x, mouse_y;
	math::float2(App->input->GetMouseX(), App->input->GetMouseY());

	float normalized_x = -(1.0f - (float(mouse_x) * 2.0f) / width);
	float normalized_y = 1.0f - (float(mouse_y) * 2.0f) / height;

	Ray = this->frustum.UnProjectLineSegment(normalized_x, normalized_y);

	float distance;
	GameObject* hit = App->level->CastRay(Ray, distance);

	if (hit != nullptr && hit_point != nullptr)
		*hit_point = Ray.GetPoint(distance);

	return hit;
}

void ComponentCamera::DrawFrustum()
{
	frustum.Transform(parent->transform->GetGlobalTransformationMatrix().Transposed());

	//glPushMatrix();
	//glMultMatrixf(*parent->transform->GetGlobalTransformationMatrix().v);
	App->renderer3D->DrawFrustum(frustum, Red);
	//glPopMatrix();
}

const float ComponentCamera::GetNearPlane() const
{
	return frustum.NearPlaneDistance();
}

const float ComponentCamera::GetFarPlane() const
{
	return frustum.FarPlaneDistance();
}

const vec ComponentCamera::GetPos() const
{
	return frustum.Pos();
}

bool ComponentCamera::GetCullingActive()
{
	return CullingActive;
}

void ComponentCamera::SetNearPlane(float value)
{
	frustum.SetViewPlaneDistances(value, frustum.FarPlaneDistance());
	near_plane = frustum.NearPlaneDistance();
}

void ComponentCamera::SetFarPlane(float value)
{
	frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), value);
	far_plane = frustum.FarPlaneDistance();
}

void ComponentCamera::SetHFov(float value)
{
	frustum.SetVerticalFovAndAspectRatio(frustum.AspectRatio(), (value * DEGTORAD));
	horizontal_fov = frustum.HorizontalFov();
}

void ComponentCamera::SetVFov(float value)
{
	frustum.SetVerticalFovAndAspectRatio((value * DEGTORAD), frustum.AspectRatio());
	vertical_fov = frustum.VerticalFov();
}

bool ComponentCamera::CheckIntersection(const math::AABB& refBox) const
{
	return frustum.Intersects(refBox);
}

void ComponentCamera::ComponentEditor()
{
	float new_near_plane = near_plane;
	if (ImGui::DragFloat("Close Plane", &new_near_plane, 0.2f))
		SetNearPlane(new_near_plane);

	float new_far_plane = far_plane;
	if (ImGui::DragFloat("Far Plane", &new_far_plane, 0.2f))
		SetFarPlane(new_far_plane);

	float new_HFov = horizontal_fov;
	if (ImGui::DragFloat("Horizontal FOV", &new_HFov, 0.2f))
		SetHFov(new_HFov);

	float new_VFov = vertical_fov;
	if (ImGui::DragFloat("Vertical FOV", &new_VFov, 0.2f))
		SetVFov(new_VFov);

	if (ImGui::Checkbox("Frustum Culling", &CullingActive));
}

const std::list<GameObject*>* ComponentCamera::GetDrawList() const
{
	return &game_objects_draw;
}

/*void ComponentCamera::OnSave(Config& config) const
{
	config.AddFloat("FrustumFar", frustum.FarPlane());
	config.AddFloat("FrustumNear", frustum.NearPlane());
	config.AddFloat("FrustumHfov", frustum.HorizontalFov());
	config.AddFloat("FrustumVfov", frustum.VerticalFov());
}

void ComponentCamera::OnLoad(Config * config)
{
	frustum.nearPlaneDistance = config->GetFloat("FrustumNear", 0.1f);
	frustum.farPlaneDistance = config->GetFloat("FrustumFar", 1000.f);
	frustum.horizontalFov = config->GetFloat("FrustumHfov", 1.f);
	frustum.verticalFov = config->GetFloat("FrustumVfov", 1.f);
	projection_changed = true;
	// TODO: Set frustum.pos / front / up to current transformation

}
*/