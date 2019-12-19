#include "Platform.h"
#include "Graphics.h"
#include "Maths.h"

//TODO: Regulate "normal" graphical pipeline
//	- Buffer spheres, cubes, cylinders
//	- Blinn-Phong lighting
//	- Provide GUI for world matrix and camera control

//TODO: Platform
//	- Input events
//	- API
//	- Internal error handling
//	- Timing
//	- Better memory management(?)

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

		Matrix4x4 model = identity();
		translate(model, Vector3(0.0f, 0.2f, 0.0f));
		Matrix4x4 view = look_at(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f));
		Matrix4x4 projection = perspective(90.0f, get_window_aspect_ratio(), 0.1f, 10.0f);

		use_shader(shader);
		set_model_matrix(shader, model);
		set_view_matrix(shader, view);
		set_projection_matrix(shader, projection);

		set_window_clear_colour(Vector3(0.98f, 0.85f, 0.86f));

		//Main loop
		while (!should_window_close()) 
		{
			handle_input();

			begin_render();

			use_shader(shader);
			draw(triangle);
			
			swap_window_buffers();
		}
		//ReleaseDC(window, window_device_context);
		shutdown_platform();
		return 0;
	}
	else return 1;
}