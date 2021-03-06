#pragma once
#include "Platform.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"
#include "Landscape.h"
#include "Camera.h"

struct UI_Parameters
{
	int* fps;
	bool* render_pines;
	bool* render_rowans;
	bool* render_wireframes;
	bool* render_depth_buffer;
	bool* render_density_field_slice;
	float* camera_position;
	float* camera_forward;
	float* camera_sensitivity;
	float* fov;
};

bool initialise_ui();
void handle_ui(UI_Parameters parameters);
void render_ui();
UI_Parameters initialise_ui_parameter_pointers(Landscape_Data* landscape, Camera* camera, int* fps, bool* render_wireframes, bool* render_depth_buffer,
	bool* render_density_field_slice, float* fov);