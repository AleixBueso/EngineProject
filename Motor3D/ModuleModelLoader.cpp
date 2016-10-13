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
	GameObject* new_object = App->gameobject_manager->CreateGameObject();
	new_object->name = "Loaded fbx";

	bool ret = true;

	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
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
				glBufferData(GL_ARRAY_BUFFER, sizeof(float2) * mesh->num_UVs, mesh->UVs, GL_STATIC_DRAW);

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

/*void ModuleModelLoader::LoadMesh(const aiMesh* mesh, const aiScene* scene)
{
	MyMesh* new_mesh = new MyMesh();

	//vertices
	glGenBuffers(1, (GLuint*)&(new_mesh->id_vertices));

	new_mesh->num_vertices = mesh->mNumVertices;
	new_mesh->vertices = new float[new_mesh->num_vertices * 3];
	memcpy(new_mesh->vertices, mesh->mVertices, sizeof(float) * new_mesh->num_vertices * 3);
	LOG("New mesh with %d vertices", new_mesh->num_vertices);

	glBindBuffer(GL_ARRAY_BUFFER, new_mesh->id_vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->num_vertices * 3, new_mesh->vertices, GL_STATIC_DRAW);



	//indices
	if (mesh->HasFaces())
	{
		glGenBuffers(1, (GLuint*)&(new_mesh->id_indices));


		new_mesh->num_indices = mesh->mNumFaces * 3;
		new_mesh->indices = new uint[new_mesh->num_indices]; // assume each face is a triangle
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			if (mesh->mFaces[i].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");

			}

			else
				memcpy(&new_mesh->indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, new_mesh->id_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * new_mesh->num_indices, new_mesh->indices, GL_STATIC_DRAW);

	}


	//normals

	if (mesh->HasNormals())
	{
		glGenBuffers(1, (GLuint*)&(new_mesh->id_normals));

		new_mesh->num_normals = new_mesh->num_vertices;
		new_mesh->normals = new float[new_mesh->num_normals * 3];
		memcpy(new_mesh->normals, mesh->mNormals, sizeof(float) * new_mesh->num_normals * 3);

		glBindBuffer(GL_ARRAY_BUFFER, new_mesh->id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->num_normals * 3, new_mesh->normals, GL_STATIC_DRAW);
	}

	if (mesh->HasTextureCoords(0))
	{
		glGenBuffers(1, (GLuint*)&(new_mesh->id_texture_coords));

		new_mesh->num_texture_coords = new_mesh->num_vertices;
		new_mesh->texture_coords = new float[new_mesh->num_texture_coords * 2];

		aiVector3D* vector = mesh->mTextureCoords[0];

		for (uint i = 0; i < new_mesh->num_texture_coords * 2; i += 2)
		{
			new_mesh->texture_coords[i] = vector->x;
			new_mesh->texture_coords[i + 1] = vector->y;
			vector++;
		}

		glBindBuffer(GL_ARRAY_BUFFER, new_mesh->id_texture_coords);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * new_mesh->num_texture_coords * 2, new_mesh->texture_coords, GL_STATIC_DRAW);
	}

	Meshes.push_back(new_mesh);

}*/

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

void ModuleModelLoader::CreateCube()
{
	uint my_id = 5;

	glLineWidth(2.0f);
	glBegin(GL_TRIANGLES);

	//-----------------------
	glTexCoord2f(0, 0);
	glVertex3f(0.f, 0.f, 0.f);
	glTexCoord2f(0, 1);
	glVertex3f(0.f, 2.f, 0.f);
	glTexCoord2f(1, 0);
	glVertex3f(0.f, 0.f, 2.f);

	glTexCoord2f(0, 1);
	glVertex3f(0.f, 2.f, 0.f);
	glTexCoord2f(1, 1);
	glVertex3f(0.f, 2.f, 2.f);
	glTexCoord2f(1, 0);
	glVertex3f(0.f, 0.f, 2.f);

	//-----------------------
	glTexCoord2f(0, 0);
	glVertex3f(0.f, 2.f, 0.f);
	glTexCoord2f(1, 1);
	glVertex3f(-2.f, 2.f, 2.f);
	glTexCoord2f(0, 1);
	glVertex3f(0.f, 2.f, 2.f);

	glTexCoord2f(1, 0);
	glVertex3f(0.f, 2.f, 0.f);
	glTexCoord2f(1, 1);
	glVertex3f(-2.f, 2.f, 0.f);
	glTexCoord2f(1, 1);
	glVertex3f(-2.f, 2.f, 2.f);

	//-----------------------
	glTexCoord2f(0, 0);
	glVertex3f(0.f, 0.f, 0.f);
	glTexCoord2f(1, 1);
	glVertex3f(-2.f, 2.f, 0.f);
	glTexCoord2f(1, 0);
	glVertex3f(0.f, 2.f, 0.f);

	glTexCoord2f(0, 1);
	glVertex3f(0.f, 0.f, 0.f);
	glTexCoord2f(1, 1);
	glVertex3f(-2.f, 0.f, 0.f);
	glTexCoord2f(1, 0);
	glVertex3f(-2.f, 2.f, 0.f);

	//-----------------------
	glTexCoord2f(0, 0);
	glVertex3f(-2.f, 0.f, 0.f);
	glTexCoord2f(1, 1);
	glVertex3f(-2.f, 2.f, 2.f);
	glTexCoord2f(1, 0);
	glVertex3f(-2.f, 2.f, 0.f);
	
	glTexCoord2f(0, 1);
	glVertex3f(-2.f, 0.f, 0.f);
	glTexCoord2f(1, 1);
	glVertex3f(-2.f, 0.f, 2.f);
	glTexCoord2f(1, 0);
	glVertex3f(-2.f, 2.f, 2.f);
	
	//-----------------------
	glTexCoord2f(0, 0);	
	glVertex3f(-2.f, 0.f, 2.f);
	glTexCoord2f(0, 1);
	glVertex3f(0.f, 0.f, 2.f);
	glTexCoord2f(1, 1);
	glVertex3f(0.f, 2.f, 2.f);
	
	glTexCoord2f(0, 0);
	glVertex3f(-2.f, 0.f, 2.f);
	glTexCoord2f(1, 1);
	glVertex3f(0.f, 2.f, 2.f);
	glTexCoord2f(0, 1);
	glVertex3f(-2.f, 2.f, 2.f);

	//-----------------------

	glTexCoord2f(0, 0);
	glVertex3f(0.f, 0.f, 0.f);
	glTexCoord2f(1, 0);
	glVertex3f(0.f, 0.f, 2.f);
	glTexCoord2f(0, 1);
	glVertex3f(-2.f, 0.f, 0.f);
	
	glTexCoord2f(0, 1);
	glVertex3f(0.f, 0.f, 2.f);
	glTexCoord2f(1, 1);
	glVertex3f(-2.f, 0.f, 2.f);
	glTexCoord2f(1, 0);
	glVertex3f(-2.f, 0.f, 0.f);
	glEnd();
	
	glBindBuffer(GL_VERTEX_ARRAY, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glLineWidth(1.0f);

}