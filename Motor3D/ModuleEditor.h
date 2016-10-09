#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleEditor : public Module
{
public:
	ModuleEditor(Application* app, bool start_enabled = true);
	~ModuleEditor();

	bool Start();

	update_status Update(float dt);

	void CreateMenu();

	void CreateHierarchy();

	bool CleanUp();

	bool ShowTestWindow = false;
	bool AboutWindow = false;
	bool ShowGameObjects = false;

};
