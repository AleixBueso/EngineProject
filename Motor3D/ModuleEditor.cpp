#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "Imgui\imgui.h"
#include "Glew\include\glew.h"
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
		if(ImGui::BeginMenu("Tools"))
			ImGui::EndMenu();

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Test Window"))
				ShowTestWindow = !ShowTestWindow;

			if (ImGui::MenuItem("Show Textures"))
				App->model_loader->texture_enabled = !App->model_loader->texture_enabled;

			ImGui::EndMenu();
		}
		
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

		if (ImGui::MenuItem("Quit"))
			return UPDATE_STOP;

		ImGui::EndMainMenuBar();
	}
	
	if(ShowTestWindow)
		ImGui::ShowTestWindow();

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

	return UPDATE_CONTINUE;
};