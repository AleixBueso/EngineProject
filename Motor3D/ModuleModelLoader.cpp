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

	//ilLoadImage("Images\Lenna.png");
	//GLubyte LeenaImage;

	{
		GLubyte checkImage[120][120][4];
		for (int i = 0; i < 120; i++) {
			for (int j = 0; j < 120; j++) {
				int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
				checkImage[i][j][0] = (GLubyte)c;
				checkImage[i][j][1] = (GLubyte)c;
				checkImage[i][j][2] = (GLubyte)c;
				checkImage[i][j][3] = (GLubyte)255;
			}

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glGenTextures(1, &ImageName);
			glBindTexture(GL_TEXTURE_2D, ImageName);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 120, 120, 0, GL_RGBA, GL_UNSIGNED_BYTE, "Images\Lenna.png");
		}
	}

	return ret;
};

bool ModuleModelLoader::CleanUp()
{
	LOG("Unloading ModelLoader");

	aiDetachAllLogStreams();
	glBindTexture(GL_TEXTURE_2D, 0);

	return true;
};

update_status ModuleModelLoader::Update(float dt)
{
	for (uint i = 0; i < Meshes.size(); i++)
		App->renderer3D->DrawMesh(Meshes[i]);


	if (texture_enabled)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);


	CreateCube();
	glBindTexture(GL_TEXTURE_2D, ImageName);



	return UPDATE_CONTINUE;
};

bool ModuleModelLoader::LoadModel(const char* full_path)
{
bool ret = true;

/*	const aiScene* scene = aiImportFile(full_path, aiProcessPreset_TargetRealtime_MaxQuality);	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			MyMesh m;
			m.num_vertices = scene->mMeshes[i]->mNumVertices;
			m.vertices = new float[m.num_vertices * 3];
			memcpy(m.vertices, scene->mMeshes[i]->mVertices, sizeof(float) * m.num_vertices * 3);
			LOG("New mesh with %d vertices", m.num_vertices);

			if (scene->mMeshes[i]->HasFaces())
			{
				m.num_indices = scene->mMeshes[i]->mNumFaces * 3;
				m.indices = new uint[m.num_indices]; // assume each face is a triangle
				for (uint k = 0; k < scene->mMeshes[i]->mNumFaces; ++k)
				{
					if (scene->mMeshes[k]->mFaces[k].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}

					else
						memcpy(&m.indices[k * 3], scene->mMeshes[i]->mFaces[k].mIndices, 3 * sizeof(uint));
				}
			}
		}

		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s", full_path);
		ret = false;
	}
*/
	return ret;
}

void ModuleModelLoader::CreateCube()
{
	uint my_id = 1;

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

}

vector<MyMesh> ModuleModelLoader::Load(const char* path)
{

	vector<MyMesh> ret;
	char* buff;
	uint size = 1000;

	if (size == 0)
	{
		LOG("Error loading %s", path);
		return ret;
	}

	const aiScene* scene = aiImportFileFromMemory(buff, size, aiProcessPreset_TargetRealtime_MaxQuality, NULL);

	if (scene != nullptr && scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* mesh_to_load = scene->mMeshes[i];
			MyMesh mesh;

			//Vertices ------------------------------------------------------------------------------------------------------
			mesh.num_vertices = mesh_to_load->mNumVertices;
			mesh.vertices = new float[mesh.num_vertices * 3];
			memcpy(mesh.vertices, mesh_to_load->mVertices, sizeof(float)*mesh.num_vertices * 3);
			LOG("Mesh Loaded with %d vertices", mesh.num_vertices);

			//Load buffer to VRAM
			glGenBuffers(1, (GLuint*)&(mesh.id_vertices));
			glBindBuffer(GL_ARRAY_BUFFER, mesh.id_vertices);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * mesh.num_vertices, mesh.vertices, GL_STATIC_DRAW);

			//Indices --------------------------------------------------------------------------------------------------------
			if (mesh_to_load->HasFaces())
			{
				mesh.num_indices = mesh_to_load->mNumFaces * 3;
				mesh.indices = new uint[mesh.num_indices];
				for (uint f = 0; f < mesh_to_load->mNumFaces; f++)
				{
					if (mesh_to_load->mFaces[f].mNumIndices != 3)
					{
						LOG("WARNING: geometry with face != 3 indices is trying to be loaded");
					}
					else
					{
						memcpy(&mesh.indices[f * 3], mesh_to_load->mFaces[f].mIndices, 3 * sizeof(uint));
					}
				}
			}

			//Load indices buffer to VRAM
			glGenBuffers(1, (GLuint*)&(mesh.id_indices));
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.id_indices);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh.num_indices, mesh.indices, GL_STATIC_DRAW);

			//Load UVs -----------------------------------------------------------------------------------------------------------------------
			if (mesh_to_load->HasTextureCoords(0))
			{
				mesh.num_uvs = mesh_to_load->mNumVertices; //Same size as vertices
				mesh.uvs = new float[mesh.num_uvs * 2];
				for (int uvs_item = 0; uvs_item < mesh.num_uvs; uvs_item++)
				{
					memcpy(&mesh.uvs[uvs_item * 2], &mesh_to_load->mTextureCoords[0][uvs_item].x, sizeof(float));
					memcpy(&mesh.uvs[(uvs_item * 2) + 1], &mesh_to_load->mTextureCoords[0][uvs_item].y, sizeof(float));
				}

				glGenBuffers(1, (GLuint*)&(mesh.id_uvs));
				glBindBuffer(GL_ARRAY_BUFFER, mesh.id_uvs);
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * mesh.num_uvs, mesh.uvs, GL_STATIC_DRAW);
			}

			//Load Textures --------------------------------------------------------------------------------------------------------------------
			/*const aiMaterial* material = scene->mMaterials[i];

			if (material != nullptr)
			{
			aiString texture_path;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path) == AI_SUCCESS) //For now only first diffuse texture
			{
			char* texture_buffer;
			if (App->file_system->Load(texture_path.data, &texture_buffer) != 0)
			{
			LOG("Texture load correctly");
			}
			}
			else
			{
			LOG("Error when loading texture of mesh %s", mesh_to_load->mName.data);
			}
			}*/


			ret.push_back(mesh);
		}
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene %s. ERROR: %s", path, aiGetErrorString());
	}

	delete[] buff;

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