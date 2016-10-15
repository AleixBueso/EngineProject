#pragma once
#include "Globals.h"
#include "Module.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "GameObject.h"

#include "Assimp\include\cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")


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

	//Normals
	uint id_normals = 0;
	uint num_normals = 0;
	float* normals = nullptr;

	//UVs
	uint id_UVs = 0;
	uint num_UVs = 0;
	math::float2* UVs = nullptr;

	//Texture
	uint id_texture_coords = 0;
	uint num_texture_coords = 0;
	float* texture_coords = nullptr;
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

	GameObject* LoadPath(const char* full_path);

	bool Load(const char* path);

	void LoadMesh(MyMesh* mesh);

	MyMesh* LoadMesh2(const aiMesh* mesh, const aiScene* scene, GameObject* new_gameobject);

	uint LoadTexture(const char* path);

	GameObject* LoadNode(const aiNode* node, const aiScene* scene, GameObject* parent);

	std::vector<MyMesh*> Meshes;

	uint ImageName;
	bool texture_enabled = true;
	GameObject* loader_it = nullptr;
	


};

