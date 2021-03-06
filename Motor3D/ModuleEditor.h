#pragma once
#include "Module.h"
#include "Globals.h"
#include "GameObject.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Start();

	update_status Update(float dt);

	void AttributeEditor();

	void ShowChilds(GameObject* parent);
	void CreateHierarchy();


	bool CleanUp();

	bool ShowTestWindow = false;
	bool AboutWindow = false;
	bool ShowGameObjects = true;
	GameObject* SelectedObject = NULL;

};
