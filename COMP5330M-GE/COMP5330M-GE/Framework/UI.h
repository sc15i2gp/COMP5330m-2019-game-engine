#pragma once
#include "Platform.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_win32.h"
#include "Terrain/Landscape.h"
#include "Rendering/Camera.h"

struct UI_Parameters
{
	int* fps;
	bool* render_pines;
	bool* render_rowans;
	bool* render_wireframes;
	float* camera_position;
	float* camera_forward;
	float* camera_sensitivity;
};

bool initialise_ui();
void handle_ui(UI_Parameters parameters);
void render_ui();
UI_Parameters initialise_ui_parameter_pointers(Landscape_Data* landscape, Camera* camera, int* fps, bool* render_wireframes);