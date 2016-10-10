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
	//MyMesh mesh;
	return false;

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

void ModuleModelLoader::Load(const char* path)
{
	const aiScene* scene = aiImportFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			MyMesh m;
			//Vertices
			m.num_vertices = scene->mMeshes[0]->mNumVertices;
			m.vertices = new float[m.num_vertices * 3];
			memcpy(m.vertices, scene->mMeshes[0]->mVertices, sizeof(float) * m.num_vertices * 3);
			LOG("New mesh with %d vertices", m.num_vertices);
			aiReleaseImport(scene);

			if (scene->mMeshes[0]->HasFaces())
			{
				m.num_indices = scene->mMeshes[0]->mNumFaces * 3;
				m.indices = new uint[m.num_indices]; // assume each face is a triangle
				for (uint i = 0; i < scene->mMeshes[0]->mNumFaces; ++i)
				{
					if (scene->mMeshes[0]->mFaces[i].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}

					else
					{
						memcpy(&m.indices[i * 3], scene->mMeshes[0]->mFaces[i].mIndices, 3 * sizeof(uint));
					}

				}
			}

			uint my_id = 0;
			glGenBuffers(1, (GLuint*) &(my_id));
			glBindBuffer(GL_ARRAY_BUFFER, my_id);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m.num_vertices * 3, m.vertices, GL_STATIC_DRAW);
		}
	}
	else
		LOG("Error loading scene %s", path);
}

uint ModuleModelLoader::LoadTexture(const char* path)
{
	ILuint id;
	ilGenImages(1, &id);
	ilBindImage(id);
	ilLoadImage(path);

	return ilutGLBindTexImage();

}