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

bool ModuleModelLoader::LoadModel(const char* full_path)
{
	//MyMesh mesh;
	return false;

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

void ModuleModelLoader::LoadMesh(MyMesh* mesh)
{
	// Generate VBO to send all this mesh information to the graphics card

	// Buffer for vertices
	glGenBuffers(1, (GLuint*) &(mesh->id_vertices));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, mesh->vertices, GL_STATIC_DRAW);

	// Buffer for indices
	glGenBuffers(1, (GLuint*) &(mesh->id_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_indices, mesh->indices, GL_STATIC_DRAW);

	// Buffer for normals
	if (mesh->normals != nullptr)
	{
		glGenBuffers(1, (GLuint*) &(mesh->id_normals));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, mesh->normals, GL_STATIC_DRAW);
	}

	// Buffer for vertex colors
	/*if (mesh->colors != nullptr)
	{
		glGenBuffers(1, (GLuint*) &(mesh->id_colors));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo_colors);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, mesh->colors, GL_STATIC_DRAW);
	}
	*/
	// Buffer for texture coords
	if (mesh->texture_coords != nullptr)
	{
		glGenBuffers(1, (GLuint*) &(mesh->id_texture_coords));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_texture_coords);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, mesh->texture_coords, GL_STATIC_DRAW);
	}
}

bool ModuleRenderer3D::LoadMeshBuffer(const MyMesh* mesh)
{
	bool ret = true;

	// Vertices
	glGenBuffers(1, (GLuint*)&(mesh->id_vertices));
	if (mesh->id_vertices == 0)
	{
		LOG("[error] Vertices buffer has not been binded!");
		ret = false;
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, &mesh->vertices, GL_STATIC_DRAW);
	}

	// Normals
	glGenBuffers(1, (GLuint*)&(mesh->id_normals));
	if (mesh->id_normals == 0)
	{
		LOG("[error] Normals buffer has not been binded!");
		ret = false;
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_normals * 3, &mesh->normals, GL_STATIC_DRAW);
	}

	// Indices
	glGenBuffers(1, (GLuint*)&(mesh->id_indices));
	if (mesh->id_indices == 0)
	{
		LOG("[error] Indices buffer has not been binded!");
		ret = false;
	}
	else
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_indices, &mesh->indices, GL_STATIC_DRAW);
	}

	/*// Texture coords
	glGenBuffers(1, (GLuint*)&(mesh->id_tex_coord));
	if (mesh->id_tex_coord == 0)
	{
		LOG("[error] Texture coordinates buffer has not been binded!");
		ret = false;
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, mesh->id_tex_coord);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_tex_coord * 2, mesh->tex_coord, GL_STATIC_DRAW);
	}*/

	return ret;
}

void ModuleModelLoader::CreateCube(MyMesh* resource)
{
	//  indices -------------------------
	/* without extra vertices for texturing:
	uint indicescw[6 * 6] =
	{
	2, 7, 6, 2, 3, 7, // front
	3, 4, 7, 3, 0, 4, // right
	1, 4, 0, 1, 5, 4, // back
	2, 5, 1, 2, 6, 5, // left
	1, 3, 2, 1, 0, 3, // top
	7, 5, 6, 7, 4, 5  // bottom
	};*/

	uint indicescw[6 * 6] =
	{
		2, 7, 6, 2, 3, 7, // front
		11, 9, 10, 11, 8, 9, // right
		1, 4, 0, 1, 5, 4, // back
		15, 13, 12, 15, 14, 13, // left
		1, 3, 2, 1, 0, 3, // top
		7, 5, 6, 7, 4, 5  // bottom
	};

	resource->num_indices = 6 * 6;
	uint bytes = sizeof(uint) * resource->num_indices;
	resource->indices = new uint[resource->num_indices];
	memcpy(resource->indices, indicescw, bytes);

	//  vertices ------------------------
	float vertices[16 * 3] =
	{
		0.5f,  0.5f,  0.5f, // 0
		-0.5f,  0.5f,  0.5f, // 1
		-0.5f,  0.5f, -0.5f, // 2
		0.5f,  0.5f, -0.5f, // 3

		0.5f, -0.5f,  0.5f, // 4
		-0.5f, -0.5f,  0.5f, // 5
		-0.5f, -0.5f, -0.5f, // 6
		0.5f, -0.5f, -0.5f, // 7

							// add repeated vertices for proper texturing
							0.5f,  0.5f,  0.5f,  // 8
							0.5f, -0.5f,  0.5f,  // 9
							0.5f, -0.5f, -0.5f,  //10
							0.5f,  0.5f, -0.5f,  //11

							-0.5f,  0.5f,  0.5f,  //12
							-0.5f, -0.5f,  0.5f,  //13
							-0.5f, -0.5f, -0.5f,  //14
							-0.5f,  0.5f, -0.5f,  //15
	};

	resource->num_vertices = 16;
	bytes = sizeof(float) * resource->num_vertices * 3;
	resource->vertices = new float[resource->num_vertices * 3];
	memcpy(resource->vertices, vertices, bytes);

	// Load texture coords
	float texture_coords[16 * 3] =
	{
		1.f,  1.f,  0.f,
		0.f,  1.f,  0.f,
		0.f,  0.f,  0.f,
		1.f,  0.f,  0.f,

		1.f,  0.f,  0.f,
		0.f,  0.f,  0.f,
		0.f,  1.f,  0.f,
		1.f,  1.f,  0.f,

		// extra coords for left - right
		1.f,  1.f,  0.f,
		0.f,  1.f,  0.f,
		0.f,  0.f,  0.f,
		1.f,  0.f,  0.f,

		0.f,  1.f,  0.f,
		1.f,  1.f,  0.f,
		1.f,  0.f,  0.f,
		0.f,  0.f,  0.f,
	};

	resource->texture_coords = new float[resource->num_vertices * 3];
	memcpy(resource->texture_coords, texture_coords, bytes);

	// AABB
	//resource->bbox = AABB(float3(-0.5f, -0.5f, -0.5f), float3(0.5f, 0.5f, 0.5f));

	// Now generate VBOs
	LoadMesh(resource);

}