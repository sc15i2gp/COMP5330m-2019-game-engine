#include "Platform.h"
#include "Graphics.h"
#include "Maths.h"
#include "RigidBody.h"

//TODO: Regulate "normal" graphical pipeline
//	- Blinn-Phong lighting
//	- Provide GUI for world matrix and camera control

//TODO: Platform
//	- Input events
//	- API
//	- Internal error handling
//	- Timing
//	- Better memory management(?)

void print_mat(Matrix4x4 m)
{
	for (int i = 0; i < 4; ++i)
	{
		OutputDebugStringf("%f %f %f %f\n", m[0][i], m[1][i], m[2][i], m[3][i]);
	}
}
//Windows entry point
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR cmd_line, int nCmdShow)
{
	bool platform_ready = initialise_platform(instance);
	bool graphics_ready = initialise_graphics();

	if (platform_ready && graphics_ready)
	{
		GLuint shader = load_shader_program("vshader.glsl", "fshader.glsl");

		//Load triangle data to GPU
		Mesh_vertex vertices[] =
		{
			{{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},//Top
			{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },//Left
			{{ 1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } }//Right
		};

		GLuint indices[] = { 0, 1, 2 };

		Drawable triangle = buffer_mesh(vertices, 3, indices, 3);
		Drawable cylinder = buffer_cylinder_mesh(0.5f, 1.0f);

		Matrix4x4 model = identity();
		print_mat(model);
		Matrix4x4 view = look_at(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f));
		//Matrix4x4 view = identity();
		Matrix4x4 projection = perspective(90.0f, get_window_aspect_ratio(), 0.1f, 10.0f);

		use_shader(shader);
		set_model_matrix(shader, model);
		set_view_matrix(shader, view);
		set_projection_matrix(shader, projection);

		set_window_clear_colour(Vector3(0.98f, 0.85f, 0.86f));

		// Physics Testing goes here
		//RigidBody point({ 0.0,20.0,0.0 }, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 }, 10.0);
		//Vector3 gravity(0.0, -9.81 * point.mass, 0.0);
		//Vector3* forces = { &gravity };
		//for (int i = 0; i < 300; i++) {
		//	updateDisplacement(point, forces, 1, 0.01667);
		//	OutputDebugStringf("%f %f %f\n", point.displacement.x, point.displacement.y, point.displacement.z);
		//	i++;
		//}

		//Main loop
		while (!should_window_close()) 
		{
			handle_input();

			begin_render();

			use_shader(shader);
			set_model_matrix(shader, identity());
			draw(triangle);
			rotate(model, Vector3(1.0f, 0.0f, 0.0f), 0.5f);
			set_model_matrix(shader, model);
			draw(cylinder);
			
			swap_window_buffers();
		}
		//ReleaseDC(window, window_device_context);
		shutdown_platform();
		return 0;
	}
	else return 1;
}