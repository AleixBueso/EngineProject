#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "Imgui\imgui.h"
#include "Glew\include\glew.h"
#include "GameObject Manager.h"
#include "glut\glut.h"
#include "ComponentTransform.h"

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
	SelectedObject = App->gameobject_manager->root;

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
			if (ImGui::Checkbox("Test Window", &ShowTestWindow));
			if (ImGui::Checkbox("Textures", &App->model_loader->texture_enabled));

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

	AttributeEditor();

	return UPDATE_CONTINUE;
};

void ModuleEditor::AttributeEditor()
{

	ImGuiTreeNodeFlags attribute_editor_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize;
	ImGui::Begin("Attribute Editor", 0, attribute_editor_flags);

	ImGui::SetWindowSize(ImVec2(390, 600));
	ImGui::SetWindowPos(ImVec2(1000, 25));

	if (SelectedObject == NULL)
		ImGui::Text("No GameObject Selected.");

	else
	{
		// Change the name
		//char* new_name = new char[20];
		//strcpy(new_name, SelectedObject->name.data());
		ImGui::Text("Name");
		//SelectedObject->name = new_name;

		//Transformation
		if (ImGui::CollapsingHeader("Transformation"));
		{
			if (SelectedObject->transform != NULL)
				SelectedObject->transform->ComponentEditor();

			else
				ImGui::Text("No transformation component.");
		}

		// Material
		if (ImGui::CollapsingHeader("Material"));
		{
			if (SelectedObject->material != NULL)
				SelectedObject->material->ComponentEditor();

			else
				ImGui::Text("No material component.");
		}

		//Mesh
		if (ImGui::CollapsingHeader("Mesh"));
		{
			if (SelectedObject->mesh != nullptr)
			{
				if (SelectedObject->mesh != NULL)
					SelectedObject->mesh->ComponentEditor();
				
				else
					ImGui::Text("No mesh component.");
			}
		}
	}

	ImGui::End();
}

void ModuleEditor::ShowChilds(GameObject* parent)
{
	if (parent->childs.empty() == false)
	{
		list<GameObject*>::const_iterator it = parent->childs.begin();
		while (it != parent->childs.end())
		{
			if ((*it) == SelectedObject)
			{
				ImGuiTreeNodeFlags_Framed;
			}

			if ((*it)->childs.size() > 0)
			{
				if (ImGui::TreeNodeEx((*it)->name.data(), ImGuiTreeNodeFlags_Framed))
				{
					if (ImGui::IsItemClicked())
					{
						SelectedObject = (*it);
					}
					ShowChilds(*it);
					ImGui::TreePop();
				}
			}

			else
			{
				if (ImGui::TreeNodeEx((*it)->name.data(), ImGuiTreeNodeFlags_Leaf))
				{
					if (ImGui::IsItemClicked())
					{
						SelectedObject = (*it);
					}
					ImGui::TreePop();
				}
			}
			++it;
		}
	}
		
}

void ModuleEditor::CreateHierarchy()
{
	ImGuiTreeNodeFlags outliner_flags = ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize;
	ImGui::Begin("Outliner", 0, outliner_flags);

	ImGui::Text("Click the Button to create an empty GameObject");

	if (ImGui::Button("CreateGameObject"))
		App->gameobject_manager->CreateGameObject(SelectedObject);

	ImGui::Text("Tree: ----------------------------");

	if (App->gameobject_manager->root == NULL)
		ImGui::Text("No GameObjects on the Scene.");

	else
	{
		if (ImGui::TreeNodeEx(App->gameobject_manager->root->name.data(), ImGuiTreeNodeFlags_Framed))
		{
			if (App->gameobject_manager->root == SelectedObject)
			{
				ImGuiTreeNodeFlags_Framed;
			}

			if (ImGui::IsItemClicked())
			{
				SelectedObject = (App->gameobject_manager->root);
			}

			if(App->gameobject_manager->root->childs.empty() == false)
				ShowChilds(App->gameobject_manager->root);

			ImGui::TreePop();
		}
	}
	ImGui::End();
}

