#include "Globals.h"
#include "Application.h"
#include "ModuleModelLoader.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

#include "Glew\include\glew.h"
#include "glut\glut.h"

#include "Devil/include/il.h"
#include "Devil/include/ilut.h"
#pragma comment ( lib, "Devil/libx86/DevIL.lib" )
#pragma comment ( lib, "Devil/libx86/ILU.lib" )
#pragma comment ( lib, "Devil/libx86/ILUT.lib" )

#include "Assimp\include\cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"
#pragma comment (lib, "Assimp/libx86/assimp.lib")

ModuleModelLoader::ModuleModelLoader(Application* app, bool start_enabled) : Module(app, start_enabled)
{};

ModuleModelLoader::~ModuleModelLoader()
{
};

bool ModuleModelLoader::Init()
{
	aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	//Initialize Devil
	ilInit();
	iluInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	return true;
}

bool ModuleModelLoader::Start()
{
	LOG("Loading ModelLoader");
	bool ret = true;

	return ret;
};

bool ModuleModelLoader::CleanUp()
{
	LOG("Unloading ModelLoader");

	for (uint i = 0; i < Meshes.size(); i++)
		delete Meshes[i];

	Meshes.clear();

	aiDetachAllLogStreams();
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
};

update_status ModuleModelLoader::Update(float dt)
{
	if (texture_enabled)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);

	return UPDATE_CONTINUE;
};

void ModuleModelLoader::LoadPath(const char* full_path)
{
	GameObject* parent = nullptr;
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		aiNode* root_node = scene->mRootNode;

		for (uint i = 0; i < root_node->mNumChildren; i++)
		{
			LoadNode(root_node, scene, parent);
		}
		aiReleaseImport(scene);
	}

}

bool ModuleModelLoader::Load(const char* path)
{
	bool ret = true;

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		GameObject* new_object = App->gameobject_manager->CreateGameObject();
		new_object->name = "Loaded fbx";

		for (uint i = 0; i < scene->mNumMeshes; i++)
		{

			ComponentMesh* comp_mesh = (ComponentMesh*)new_object->CreateComponent(component_type::COMPONENT_MESH, 0);

			LOG("[start] New mesh ----------------------------------------------------");
			MyMesh* mesh = new MyMesh();

			glGenBuffers(1, (GLuint*)&(mesh->id_vertices));
			glGenBuffers(1, (GLuint*)&(mesh->id_indices));
			glGenBuffers(1, (GLuint*)&(mesh->id_normals));
			glGenBuffers(1, (GLuint*)&(mesh->id_UVs));


			// Copying vertices
			mesh->num_vertices = scene->mMeshes[i]->mNumVertices;
			mesh->vertices = new float[mesh->num_vertices];
			memcpy(mesh->vertices, scene->mMeshes[i]->mVertices, sizeof(float) * mesh->num_vertices);
			LOG("New mesh with %d vertices", mesh->num_vertices);


			// Copying indicies (faces on Assimp)
			if (scene->mMeshes[i]->HasFaces())
			{
				mesh->num_indices = scene->mMeshes[i]->mNumFaces * 3;
				mesh->indices = new uint[mesh->num_indices]; //Each face is a triangle
				for (uint j = 0; j < scene->mMeshes[i]->mNumFaces; ++j)
				{
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
					{
						LOG("[warning], geometry face without 3 indices!");
					}
					else
					{
						memcpy(&mesh->indices[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint));
					}
				}
			}


			// Copying normals
			mesh->num_normals = scene->mMeshes[i]->mNumVertices;
			mesh->normals = new float[mesh->num_normals];
			memcpy(mesh->normals, scene->mMeshes[i]->mNormals, sizeof(float) * mesh->num_normals);
			LOG("New mesh with %d normals", mesh->num_normals);

			uint uv_id = 0;
			if (scene->mMeshes[i]->HasTextureCoords(uv_id))
			{
				mesh->num_UVs = scene->mMeshes[i]->mNumVertices;
				mesh->UVs = new float2[mesh->num_UVs];
				for (int k = 0; k < mesh->num_UVs; k++)
				{
					memcpy(&mesh->UVs[k], &scene->mMeshes[i]->mTextureCoords[uv_id][k].x, sizeof(float2));
					memcpy(&mesh->UVs[k + 1], &scene->mMeshes[i]->mTextureCoords[uv_id][k + 1].y, sizeof(float2));
				}


				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_indices, mesh->indices, GL_STATIC_DRAW);

				glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, mesh->vertices, GL_STATIC_DRAW);

				glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_normals * 3, mesh->normals, GL_STATIC_DRAW);

				glBindBuffer(GL_ARRAY_BUFFER, mesh->id_UVs);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float2) * mesh->num_UVs * 2, mesh->UVs, GL_STATIC_DRAW);

				//Load Materials
				if (scene->HasMaterials())
				{
					aiMaterial* material = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];

					aiString path;
					material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

					if (path.length > 0)
					{
						ComponentMaterial* comp_material = (ComponentMaterial*)new_object->CreateComponent(component_type::COMPONENT_MATERIAL, 0);

						comp_material->texture_id = App->model_loader->LoadTexture(path.data);
					}
				}
			}
			comp_mesh->mesh = mesh;
			Meshes.push_back(mesh);
		}


	}

	else
	{
		ret = false;
		LOG("Error loading scene %s", path);
	}

	return ret;
}

uint ModuleModelLoader::LoadTexture(const char* path)
{
	ILuint id;
	ilGenImages(1, &id);
	ilBindImage(id);
	ilLoadImage(path);

	return ilutGLBindTexImage();

}

GameObject* ModuleModelLoader::LoadNode(const aiNode* node, const aiScene* scene, GameObject* parent)
{			
	
	GameObject* game_object = App->gameobject_manager->CreateGameObject(parent);
	game_object->name = "Loaded fbx";
	//char* tmp[256];
	//memcpy(&game_object->name, node->mName.data, node->mName.length + 1);

	//transformation
	if (scene->HasMeshes() == true)
	{
		for (uint i = 0; i < node->mNumMeshes; i++)
		{
			//LoadMesh2(scene->mMeshes[node->mMeshes[i]], scene, game_object);
		}
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		//game_object->AddChild(LoadNode(node->mChildren[i], scene, game_object));
	}
	return game_object;
}

void ModuleModelLoader::LoadMesh2(const aiMesh* mesh, const aiScene* scene, GameObject* game_object)
{
	MyMesh* m = new MyMesh();

	ComponentMesh* comp_mesh = (ComponentMesh*)game_object->CreateComponent(component_type::COMPONENT_MESH, 0);
	//vertices
	glGenBuffers(1, (GLuint*)&(m->id_vertices));

	m->num_vertices = mesh->mNumVertices;
	m->vertices = new float[m->num_vertices * 3];
	memcpy(m->vertices, mesh->mVertices, sizeof(float) * m->num_vertices * 3);
	LOG("New mesh with %d vertices", m->num_vertices);

	glBindBuffer(GL_ARRAY_BUFFER, m->id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->num_vertices * 3, m->vertices, GL_STATIC_DRAW);

	//indices
	if (mesh->HasFaces())
	{
		glGenBuffers(1, (GLuint*)&(m->id_indices));


		m->num_indices = mesh->mNumFaces * 3;
		m->indices = new uint[m->num_indices]; // assume each face is a triangle
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			if (mesh->mFaces[i].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");

			}

			else
				memcpy(&m->indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m->num_indices, m->indices, GL_STATIC_DRAW);

	}

	//normals

	if (mesh->HasNormals())
	{
		glGenBuffers(1, (GLuint*)&(m->id_normals));

		m->num_normals = m->num_vertices;
		m->normals = new float[m->num_normals * 3];
		memcpy(m->normals, mesh->mNormals, sizeof(float) * m->num_normals * 3);

		glBindBuffer(GL_ARRAY_BUFFER, m->id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->num_normals * 3, m->normals, GL_STATIC_DRAW);
	}

	//tex coords
	if (mesh->HasTextureCoords(0))
	{
		glGenBuffers(1, (GLuint*)&(m->id_texture_coords));

		m->num_texture_coords = m->num_vertices;
		m->texture_coords = new float[m->num_texture_coords * 2];

		aiVector3D* vector = mesh->mTextureCoords[0];

		for (uint i = 0; i < m->num_texture_coords * 2; i += 2)
		{
			m->texture_coords[i] = vector->x;
			m->texture_coords[i + 1] = vector->y;
			vector++;
		}

		glBindBuffer(GL_ARRAY_BUFFER, m->id_texture_coords);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->num_texture_coords * 2, m->texture_coords, GL_STATIC_DRAW);
	}

	comp_mesh->mesh = m;
	
	//Load Materials
	/*if (scene->HasMaterials())
	{
		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			aiMaterial* material = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];

			aiString path;
			material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

			if (path.length > 0)
			{
				ComponentMaterial* comp_material = (ComponentMaterial*)game_object->CreateComponent(component_type::COMPONENT_MATERIAL, 0);

				comp_material->texture_id = App->model_loader->LoadTexture(path.data);
			}
		}
	}
	*/
	Meshes.push_back(m);
}