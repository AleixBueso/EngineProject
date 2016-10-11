#include "Globals.h"
#include "Application.h"
#include "ModuleModelLoader.h"

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
	for (uint i = 0; i < Meshes.size(); i++)
		App->renderer3D->DrawMesh(*Meshes[i]);


	if (texture_enabled)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);


	//CreateCube();
	//glBindTexture(GL_TEXTURE_2D, ImageName);



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
		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			LOG("[start] New mesh ----------------------------------------------------");
			MyMesh* mesh = new MyMesh();

			// Copying vertices
			mesh->num_vertices = scene->mMeshes[i]->mNumVertices;
			mesh->vertices = new float[mesh->num_vertices];
			memcpy(mesh->vertices, scene->mMeshes[i]->mVertices, sizeof(float) * mesh->num_vertices);
			LOG("New mesh with %d vertices", mesh->num_vertices);

			glGenBuffers(1, (GLuint*)&(mesh->id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertices * 3, mesh->vertices, GL_STATIC_DRAW);

			// Copying normals
			mesh->num_normals = scene->mMeshes[i]->mNumVertices;
			mesh->normals = new float[mesh->num_normals];
			memcpy(mesh->normals, scene->mMeshes[i]->mNormals, sizeof(float) * mesh->num_normals);
			LOG("New mesh with %d normals", mesh->num_normals);

			glGenBuffers(1, (GLuint*)&(mesh->id_normals));
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_normals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float)* mesh->num_normals * 3, mesh->normals, GL_STATIC_DRAW);

			// Copiying Indices
			if (scene->mMeshes[i]->HasFaces())
			{
				mesh->num_indices = scene->mMeshes[i]->mNumFaces * 3;
				mesh->indices = new uint[mesh->num_indices];
				for (unsigned int j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
				{
					if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
					{
						LOG("WARNING, Geometry with more/less than 3 faces wants to be loaded");
					}
					else
					{
						memcpy(&mesh->indices[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, sizeof(uint) * 3);
					}
				}
			}
			glGenBuffers(1, (GLuint*)&(mesh->id_indices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh->num_indices, mesh->indices, GL_STATIC_DRAW);

			//Copy UVs----------------------------------------------------------------------------------------
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
				glGenBuffers(1, (GLuint*)&(mesh->id_UVs));
				glBindBuffer(GL_ARRAY_BUFFER, mesh->id_UVs);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float2) * mesh->num_UVs, mesh->UVs, GL_STATIC_DRAW);
			}

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