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
	return UPDATE_CONTINUE;
};

GameObject* ModuleModelLoader::LoadPath(const char* full_path)
{
	GameObject* ret = nullptr;
	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene)
	{
		ret = LoadNode(scene->mRootNode, scene, App->gameobject_manager->root);
		//App->gameobject_manager->root->childs.push_back(ret);
	}

	aiReleaseImport(scene);
	scene = nullptr;
	return ret;

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
	game_object->name = node->mName.data;

	//transformation
	aiQuaternion rotation;
	aiVector3D scalar;
	aiVector3D position;

	node->mTransformation.Decompose(scalar, rotation, position);

	ComponentTransform* comp_trans = (ComponentTransform*)game_object->CreateComponent(component_type::COMPONENT_TRANSFORM, 0);
	comp_trans->SetTranslation(position.x, position.y, position.z);
	comp_trans->SetScale(scalar.x, scalar.y, scalar.z);
	comp_trans->SetRotation(rotation.x, rotation.y, rotation.z, rotation.w);
	

	if (scene->HasMeshes() == true)
	{
		for (uint i = 0; i < node->mNumMeshes; i++)
		{
			ComponentMesh* comp_mesh = (ComponentMesh*)game_object->CreateComponent(component_type::COMPONENT_MESH, 0);
			comp_mesh->mesh = LoadMesh2(scene->mMeshes[node->mMeshes[i]], scene, game_object);
		}
	}

	for (int i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene, game_object);
	}

	return game_object;
}

MyMesh* ModuleModelLoader::LoadMesh2(const aiMesh* mesh, const aiScene* scene, GameObject* new_object)
{

	//Mesh
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

	//Load Materials
	if (scene->HasMaterials())
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

		if (path.length > 0)
		{
			ComponentMaterial* comp_material = (ComponentMaterial*)new_object->CreateComponent(component_type::COMPONENT_MATERIAL, 0);
			std::string complete_path = "Game";
			std::string path_2 = path.data;
			path_2.erase(0, 2);
			complete_path = complete_path + path_2;
			LOG("%s", complete_path.data());

			comp_material->texture_id = App->model_loader->LoadTexture(complete_path.data());
			comp_material->path = path.data;
			LOG("Texture id %i Load: %s", comp_material->texture_id, path.data);
		}
	}

	Meshes.push_back(new_mesh);
	return new_mesh;
}