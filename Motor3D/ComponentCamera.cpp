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
}

ComponentCamera::~ComponentCamera()
{

}

void ComponentCamera::Update(float dt)
{
	DrawFrustum();
	/*near_plane = frustum.NearPlaneDistance();
	far_plane = frustum.FarPlaneDistance();
	horizontal_fov = frustum.HorizontalFov();
	vertical_fov = frustum.VerticalFov();*/
}

void ComponentCamera::DrawFrustum()
{
	glPushMatrix();
	glMultMatrixf(*parent->transform->GetGlobalTransformationMatrix().v);
	App->renderer3D->DrawFrustum(frustum, Red);
	glPopMatrix();
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

void ComponentCamera::SetNearPlane(float value)
{
	frustum.SetViewPlaneDistances(value, frustum.FarPlaneDistance());
}

void ComponentCamera::SetFarPlane(float value)
{
	frustum.SetViewPlaneDistances(frustum.NearPlaneDistance(), value);
}

void ComponentCamera::SetHFov(float value)
{
	frustum.SetVerticalFovAndAspectRatio(frustum.AspectRatio(), (value * DEGTORAD));
}

void ComponentCamera::SetVFov(float value)
{
	frustum.SetVerticalFovAndAspectRatio((value * DEGTORAD), frustum.AspectRatio());
}

bool ComponentCamera::CheckIntersections(const AaBox& refBox) const
{
	Vector3f vCorner[8];
	int iTotalIn = 0;
	refBox.GetVertices(vCorner); // get the corners of the box into the vCorner array
								 // test all 8 corners against the 6 sides
								 // if all points are behind 1 specific plane, we are out
								 // if we are in with all points, then we are fully in
	for (int p = 0; p < 6; ++p) {
		int iInCount = 8;
		int iPtIn = 1;
		for (int i = 0; i < 8; ++i) {
			// test this point against the planes
			if (m_plane[p].SideOfPlane(vCorner[i]) == BEHIND) {
				iPtIn = 0;
				--iInCount;
			}
		}
		// were all the points outside of plane p?
		If(iInCount == 0)
			return(OUT);
		// check if they were all on the right side of the plane
		iTotalIn += iPtIn;
	}
	// so if iTotalIn is 6, then all are inside the view
	if (iTotalIn == 6)
		return(IN);
	// we must be partly in then otherwise
	return(INTERSECT);
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
}
