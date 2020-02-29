#include "UI.h"

bool initialise_ui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(get_window());
	ImGui_ImplOpenGL3_Init("#version 460");
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	return true;
}

void render_ui()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void handle_ui(UI_Parameters parameters)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Camera");
	ImGui::Text("Position");
	ImGui::NewLine();
	ImGui::InputFloat3("##P", parameters.camera_position);
	ImGui::NewLine();
	ImGui::Text("Forward direction");
	ImGui::NewLine();
	ImGui::InputFloat3("##Q", parameters.camera_forward);
	ImGui::NewLine();
	ImGui::Text("Movement Sensitivity");
	ImGui::SliderFloat("", parameters.camera_sensitivity, 0.001f, 1.0f);
	ImGui::NewLine();
	ImGui::Text("FoV");
	ImGui::SliderFloat("##FOV", parameters.fov, 30.0f, 120.0f);
	ImGui::End();

	ImGui::Begin("Performance");
	ImGui::Text("FPS");
	ImGui::NewLine();
	ImGui::SliderInt("", parameters.fps, 24, 120);
	ImGui::End();

	ImGui::Begin("Rendering");
	ImGui::Text("Render Pines?");
	ImGui::SameLine();
	ImGui::Checkbox("##Pines", parameters.render_pines);
	ImGui::NewLine();
	ImGui::Text("Render Rowans?");
	ImGui::SameLine();
	ImGui::Checkbox("##Rowans", parameters.render_rowans);
	ImGui::NewLine();
	ImGui::Text("Render as wireframes?");
	ImGui::SameLine();
	ImGui::Checkbox("##Wireframes", parameters.render_wireframes);
	ImGui::NewLine();
	ImGui::Text("Render depth buffer?");
	ImGui::SameLine();
	ImGui::Checkbox("##Depth_buffer", parameters.render_depth_buffer);
	ImGui::End();
}

UI_Parameters initialise_ui_parameter_pointers(Landscape_Data* landscape, Camera* camera, int* fps, bool* render_wireframes, bool* render_depth_buffer, float* fov)
{
	UI_Parameters ui_parameters = {};
	ui_parameters.fps = fps;
	ui_parameters.render_pines = &landscape->render_pines;
	ui_parameters.render_rowans = &landscape->render_rowans;
	ui_parameters.render_wireframes = render_wireframes;
	ui_parameters.camera_forward = &camera->forward[0];
	ui_parameters.camera_position = &camera->position[0];
	ui_parameters.camera_sensitivity = &camera->movement_sensitivity;
	ui_parameters.render_depth_buffer = render_depth_buffer;
	ui_parameters.fov = fov;

	return ui_parameters;
}
