#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "SDL\include\SDL.h"

#include "Imgui\imgui.h"
#include "ModuleModelLoader.h"

#include "MathGeoLib\include\MathGeoLib.h"
#include "MathGeoLib\include\MathBuildConfig.h"

#include "Glew\include\glew.h"
#include "glut\glut.h"

#include "Random.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->model_loader->LoadPath("Models/Town/Street Environment_V01.fbx");
	//App->model_loader->Load("Models/Town/Street Environment_V01.fbx");
	//MyMesh* mesh = new MyMesh();
	//App->model_loader->CreateCube(mesh);

	//lenna_id = App->model_loader->LoadTexture("lenna.png");

	//App->gameobject_manager->CreateGameObject();
	

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{
	return UPDATE_CONTINUE;
}


