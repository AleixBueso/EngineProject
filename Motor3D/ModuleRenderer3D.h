#pragma once
#include "Module.h"
#include "Globals.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "Light.h"
#include "ModuleModelLoader.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"


#define MAX_LIGHTS 8

using namespace math;

struct MyMesh;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height, float fovy);

	void DrawMesh(ComponentMesh* mesh = NULL, ComponentTransform* transfrom = NULL, ComponentMaterial* material = NULL);
	bool LoadMeshBuffer(const MyMesh* mesh);
	void DrawBox(const float3 * corners, Color color);
	void RenderBoundingBox(const math::AABB & aabb, Color color);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};