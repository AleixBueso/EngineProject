#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "Imgui\imgui.h"
#include "Glew\include\glew.h"
#include "GameObject Manager.h"
#include "glut\glut.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{};

ModuleEditor::~ModuleEditor()
{
};

bool ModuleEditor::Start()
{
	LOG("Loading Editor");
	bool ret = true;

	//Set Camera Position
	App->camera->Move(vec(10.0f, 6.0f, 30.0f));
	App->camera->LookAt(vec(0, 0, 0));

	ShowTestWindow = false;

	return ret;
};

bool ModuleEditor::CleanUp()
{
	LOG("Unloading Editor");

	return true;
};

update_status ModuleEditor::Update(float dt) 
{
	//Create World
	Plane_P grid(0, 1, 0, 0);
	grid.axis = true;
	grid.Render();

	//Create the menu bar
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Tools"))
		{
			if (ImGui::MenuItem("GameObjects Editor"))
			{
				ShowGameObjects = !ShowGameObjects;
			}
				ImGui::EndMenu();
		}

		//View
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Test Window"))
				ShowTestWindow = !ShowTestWindow;

			if (ImGui::MenuItem("Show Textures"))
				App->model_loader->texture_enabled = !App->model_loader->texture_enabled;

			ImGui::EndMenu();
		}

		//Help
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Documentation"))
				App->RequestBrowser("https://github.com/AleixBueso/Motor3D/wiki");

			if (ImGui::MenuItem("Download Latest"))
				App->RequestBrowser("https://github.com/AleixBueso/Motor3D/releases");

			if (ImGui::MenuItem("Report a Bug"))
				App->RequestBrowser("https://github.com/AleixBueso/Motor3D/issues");

			if (ImGui::MenuItem("About"))
				AboutWindow = !AboutWindow;

			ImGui::EndMenu();
		}

		 //Quit
		if (ImGui::MenuItem("Quit"))
			return UPDATE_STOP;

		ImGui::EndMainMenuBar();
	}

	//TestWindow
	if (ShowTestWindow)
		ImGui::ShowTestWindow();

	//AboutWindow
	if (AboutWindow)
	{
		ImGui::Begin("About");

		ImGui::Text("3D Engine");
		ImGui::Text("Engine for Motors UPC");
		if (ImGui::BeginMenu("Libraries:"))
		{
			ImGui::Text("ImGui");
			ImGui::Text("Open GL3");
			ImGui::Text("MathGeolab");
			ImGui::Text("Bullet");
			ImGui::Text("SDL");
			ImGui::Text("Glew");
			ImGui::EndMenu();
		}
		ImGui::MenuItem("License");

		ImGui::End();
	}

	//Outliner
	if (ShowGameObjects)
	{
		CreateHierarchy();
	}


	return UPDATE_CONTINUE;
};

void ModuleEditor::CreateMenu()
{

}

void ModuleEditor::ShowChilds(GameObject* parent)
{
	if (parent->childs.empty() == false)
	{
		for (uint i = 0; i < parent->childs.size(); i++)
		{
			ImGui::Text(parent->childs[i]->name.c_str());
			ShowChilds(parent->childs[i]);
		}
	}
}

void ModuleEditor::CreateHierarchy()
{
	ImGui::Begin("Outliner");

	if (App->gameobject_manager->root == NULL)
		ImGui::Text("No GameObjects on the Scene :-(");

	else
	{
			if (ImGui::TreeNode(App->gameobject_manager->root->name.c_str()))
			{
				ShowChilds(App->gameobject_manager->root);

				ImGui::TreePop();
			}
	}

	ImGui::End();
}

