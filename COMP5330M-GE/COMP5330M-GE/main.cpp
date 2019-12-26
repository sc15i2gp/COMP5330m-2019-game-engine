#include "Platform.h"
#include "Graphics.h"
#include "Maths.h"

//TODO: Regulate "normal" graphical pipeline
//	- Blinn-Phong lighting
//	- Provide GUI for world matrix and camera control

//TODO: Platform
//	- REMOVE try-catch from alloc mem
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
		load_shader_program("vshader.glsl", "fshader.glsl");
		
		//Load triangle data to GPU
		Mesh_vertex vertices[] =
		{
			{{0.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},//Top
			{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f} },//Left
			{{ 1.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } }//Right
		};

		GLuint indices[] = { 0, 1, 2 };

		Drawable triangle = buffer_mesh(vertices, 3, indices, 3);
		Drawable cylinder = buffer_cylinder_mesh(0.5f, 1.0f, 64);

		Matrix4x4 model = identity();
		Matrix4x4 view = look_at(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f));
		Matrix4x4 projection = perspective(90.0f, get_window_aspect_ratio(), 0.1f, 10.0f);

		set_model_matrix(model);
		set_view_matrix(view);
		set_projection_matrix(projection);

		set_view_origin(Vector3(0.0f, 0.0f, 1.0f));

		set_window_clear_colour(Vector3(0.98f, 0.85f, 0.86f));

		Material emerald;
		emerald.ambient = Vector3(0.0215f, 0.1745f, 0.0215f);
		emerald.diffuse = Vector3(0.07568f, 0.61424f, 0.07568f);
		emerald.specular = Vector3(0.633f, 0.727811f, 0.633f);
		emerald.shininess = 256.f;

		Material gold;
		gold.ambient = Vector3(0.247, 0.199, 0.075);
		gold.diffuse = Vector3(0.752, 0.606, 0.226);
		gold.specular = Vector3(0.628, 0.556, 0.367);
		gold.shininess = 25.f;

		Material light_properties;
		light_properties.ambient = Vector3(0.2, 0.2, 0.2);
		light_properties.diffuse = Vector3(0.9, 0.9, 0.9);
		light_properties.specular = Vector3(1.0, 1.0, 1.0);

		activate_direction_light(0);
		set_direction_light_direction(0, Vector3(0.0f, -1.0f, 0.0f));
		set_direction_light_blinn_phong_properties(0, light_properties);

		activate_point_light(0);
		set_point_light_position(0, Vector3(0.0f, 0.0f, 2.0f));
		set_point_light_blinn_phong_properties(0, light_properties);
		set_point_light_attenuation_properties(0, 1.0, 0.14, 0.07);
		
		set_spot_light_position(0, Vector3(0.0f, 0.0f, 2.0f));
		set_spot_light_direction(0, Vector3(0.0f, 0.0f, -1.0f));
		set_spot_light_blinn_phong_properties(0, light_properties);
		set_spot_light_inner_cutoff(0, 45.0f);
		set_spot_light_outer_cutoff(0, 90.0f);
		set_spot_light_attenuation_properties(0, 1.0, 0.14, 0.07);

		//Main loop
		while (!should_window_close()) 
		{
			handle_input();

			begin_render();

			use_shader();
			set_model_matrix(identity());
			set_material(gold);
			draw(triangle);
			set_model_matrix(model);
			rotate(model, Vector3(1.0f, 0.0f, 0.0f), 0.1f);
			set_material(emerald);
			draw(cylinder);
			
			swap_window_buffers();
		}
		//ReleaseDC(window, window_device_context);
		shutdown_platform();
		return 0;
	}
	else return 1;
}