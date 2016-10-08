#pragma once
#include "Globals.h"
#include "Module.h"


struct MyMesh
{
	//Vertices
	uint id_vertices = 0;
	uint num_vertices = 0;
	float* vertices = nullptr;

	//Indices
	uint id_indices = 0;
	uint num_indices = 0;
	uint* indices = nullptr;

	//UVs
	uint id_uvs = 0;
	uint num_uvs = 0;
	float* uvs = nullptr;

	//Texture
	uint id_texture = 0;
};

class ModuleModelLoader : public Module
{
public:

	ModuleModelLoader(Application* app, bool start_enabled = true);
	~ModuleModelLoader();

	bool Init();
	bool Start();
	bool CleanUp();

	update_status Update(float dt);

	bool LoadModel(const char* full_path);

	vector<MyMesh> Load(const char* path);

	uint LoadTexture(const char* path);

	void CreateCube();

	vector<MyMesh> Meshes;

	uint ImageName;
	bool texture_enabled = false;
	


};

