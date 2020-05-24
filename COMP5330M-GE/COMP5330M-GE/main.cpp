#pragma once

#include "Framework/Platform.h"
#include "FrameWork/Graphics.h"
#include "Tools/Maths.h"
#include "Terrain/Landscape.h"
#include "Lsystems/l_system.h"
#include "DataStructures/turtle.h"
#include "Rendering/Camera.h"
#include "Framework/UI.h"
#include "./Rendering/WaterRendering.h"
#include <string>
#include <chrono>


Material light_properties =
{
	Vector3(0.2, 0.2, 0.2),
	Vector3(0.5, 0.5, 0.5),
	Vector3(0.6, 0.6, 0.6),
	0.0f
};

void buffer_camera_data_to_gpu(Camera c)
{
	/*Computes a view matrix for the camera. It takes in four paramaters.*/
	/*They are as follows: The camera position, the forward vector, expressed in negative z*/
	/*The rightward vector (facing positive X), and the upward vector (facing positive Y)*/
	Matrix4x4 view = look_at(c.getPosition(), c.getForwardVector(), c.getRightVector(), c.getUpwardVector());

	set_view_matrix(view);

	//Buffer view position to shader
	set_view_origin(c.getPosition());
}

//Render 4*4 perlin noise grid on 64*64 texture
Drawable buffer_heightmap_to_textured_quad(GLuint* heightmap_texture)
{
	Perlin_Noise_Function perlin_noise = generate_noise_function();
	GLuint heightmap_data_width = 1024;
	GLuint heightmap_data_height = 1024;

	//Buffer texture data
	GLfloat* heightmap_data = (GLfloat*)alloc_mem(heightmap_data_width*heightmap_data_height * sizeof(GLfloat));
	float length = 128.0f;
	float x_step = length / (float)heightmap_data_width;
	float y_step = length / (float)heightmap_data_height;
	//Render heightmap over range of terrain
	for (int y = 0; y < heightmap_data_height; ++y)
	{
		for (int x = 0; x < heightmap_data_width; ++x)
		{
			float f_x = (float)x * x_step;
			float f_y = (float)y * y_step;
			float noise_val = (perlin_noise(f_x, f_y) + 1.0f) / 2.0f;
			heightmap_data[y * heightmap_data_width + x] = noise_val;
		}
	}
	*heightmap_texture = buffer_texture(heightmap_data_width, heightmap_data_height, heightmap_data, GL_RED);

	//Create quad mesh
	Mesh_vertex quad_vertices[4] = {};
	//Top left
	quad_vertices[0] =
	{
		{-1.0f, 1.0f, 0.0f}, //Position
		{}, //Normal
		{0.0f, 0.0f}  //Texture coords
	};
	//Bottom left
	quad_vertices[1] =
	{
		{-1.0f, -1.0f, 0.0f}, //Position
		{}, //Normal
		{0.0f, 1.0f}  //Texture coords
	};
	//Bottom right
	quad_vertices[2] =
	{
		{1.0f, -1.0f, 0.0f}, //Position
		{}, //Normal
		{1.0f, 1.0f}  //Texture coords
	};
	//Top right
	quad_vertices[3] =
	{
		{1.0f, 1.0f, 0.0f}, //Position
		{}, //Normal
		{1.0f, 0.0f}  //Texture coords
	};
	GLuint quad_indices[] = { 0, 1, 2, 2, 3, 0 };
	Drawable quad = buffer_mesh(quad_vertices, 4, quad_indices, 6);
	dealloc_mem(heightmap_data);
	return quad;
}

long int fps_to_mspf(int fps)
{
	float spf = 1.0f / (float)fps;
	float mspf = 1000.0f*spf;
	return (long int)mspf;
}

struct timer
{
	LARGE_INTEGER clock_frequency;
	LARGE_INTEGER start_time;
	LARGE_INTEGER stop_time;
};
void start_timer(timer* t)
{
	QueryPerformanceFrequency(&t->clock_frequency);
	QueryPerformanceCounter(&t->start_time);
}

void stop_timer(timer* t)
{
	QueryPerformanceCounter(&t->stop_time);
}

long int elapsed_time(timer* t)
{
	LARGE_INTEGER elapsed;
	elapsed.QuadPart = t->stop_time.QuadPart - t->start_time.QuadPart;
	elapsed.QuadPart *= 1000;
	elapsed.QuadPart /= t->clock_frequency.QuadPart;

	return elapsed.QuadPart;
}


//Windows entry point
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR cmd_line, int nCmdShow)
{

	std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();

	//bool platform_ready = initialise_platform(instance);
	//bool graphics_ready = initialise_graphics();
	///*bool ui_ready = initialise_ui();*/

	//if (platform_ready && graphics_ready)
	//{
	//	int bp_shader = load_shader_program("Shaders/vshader.glsl", "Shaders/fshader.glsl");
	//	int terrain_shader = load_shader_program("Shaders/vshader.glsl", "Shaders/terrain_fshader.glsl");
	//	int terrain_lighting_shader = load_shader_program("Shaders/vshader.glsl", "Shaders/fshader.glsl");
	//	int heightmap_shader = load_shader_program("Shaders/heightmap_vshader.glsl", "Shaders/heightmap_fshader.glsl");

	//	Camera main_view_camera(Vector3(0.0f, 0.0f, 10.0f), Vector3(0.0f, 0.0f, 0.0f) );

	//	//main_view_camera.set_position_and_target(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 0.0f, 0.0f, 1.0f });
	//	main_view_camera.movement_sensitivity = 0.1f;
	//	set_window_clear_colour(Vector3(0.52, 0.8, 0.92));

	//	activate_direction_light(0);
	//	set_direction_light_direction(0, Vector3(0.0f, -1.0f, -1.0f));
	//	set_direction_light_blinn_phong_properties(0, light_properties);

	//	Landscape_Data landscape = create_landscape(10.0f, 10.0f, 0.01f, 10);

	//	bool dragging = false;
	//	int fps = 60;
	//	bool render_wireframes = false;
		/*UI_Parameters ui_parameters = initialise_ui_parameter_pointers(&landscape, &main_view_camera, &fps, &render_wireframes);*/


		//WaterRendering water;
		//water.renderWaterSurface();

		//timer t;
		////Main loop
		//while (!should_window_close())
		//{
		//	start_timer(&t);
		//	long int mspf = fps_to_mspf(fps);

		//	handle_input();
			/*handle_ui(ui_parameters);*/

			//if (was_mouse_button_pressed(BUTTON_LEFT) && was_mouse_moved() && !dragging)
			//{
			//	dragging = true;
			//	//main_view_camera.rotation_start(get_initial_mouse_position());
			//}
			//if (was_mouse_moved() && dragging)
			//{
			//	//main_view_camera.rotation_start(get_initial_mouse_position());
			//	main_view_camera.rotate_by_arcball(get_final_mouse_position());
			//}
			//if (!was_mouse_button_pressed(BUTTON_LEFT) && dragging)
			//{
			//	main_view_camera.rotation_end();
			//	dragging = false;
			//}

			//if (was_key_pressed(KEY_W)) main_view_camera.moveForward();
			//if (was_key_pressed(KEY_A)) main_view_camera.moveLeft();
			//if (was_key_pressed(KEY_S)) main_view_camera.moveBackward();
			//if (was_key_pressed(KEY_D)) main_view_camera.moveRight();
			//if (was_key_pressed(KEY_Q)) main_view_camera.moveUp();
			//if (was_key_pressed(KEY_E)) main_view_camera.moveDown();

			//begin_render();
			//if (render_wireframes) draw_as_wireframes();
			//else draw_as_polygons();

			//set_projection_matrix(perspective(50.0f, get_window_aspect_ratio(), 0.1f, 1000.0f));
			//set_model_matrix(identity());
			//use_shader(terrain_lighting_shader);

			//buffer_camera_data_to_gpu(main_view_camera);

			//Draw landscape
			/*landscape.draw();*/
			/*render_ui();*/

			/*Get a vlue for sine and cosine*/

			std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();

			std::chrono::duration<float> difference = end - start;
			std::chrono::duration<float, std::nano> ms = end - start;

			OutputDebugStringA(std::to_string(difference.count()).c_str());
			OutputDebugStringA("\n");
			OutputDebugStringA(std::to_string(ms.count()).c_str());
			OutputDebugStringA("\n");

			/*std::chrono::duration<std::m = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);*/
		/*	water.drawWater();*/


	//		swap_window_buffers();

	//		stop_timer(&t);
	//		long int frame_time = elapsed_time(&t);
	//		if (frame_time < mspf)
	//		{
	//			DWORD sleep_time = mspf - frame_time;
	//			Sleep(sleep_time);
	//		}
	//	}
	//	//release_drawable();
	//	//ReleaseDC(window, window_device_context);
	///*	ImGui_ImplOpenGL3_Shutdown();
	//	ImGui_ImplWin32_Shutdown();*/
	//	shutdown_platform();
	//	return 0;
	//}
	//else return 1;

}
