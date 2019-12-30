#include "Platform.h"
#include "Graphics.h"
#include "Maths.h"
#include "Landscape.h"

//TODO: Platform
//	- REMOVE try-catch from alloc mem
//	- Input events
//	- API
//	- Internal error handling
//	- Timing
//	- Better memory management(?)

//TODO: Terrain
//	- Parameterise
//	- Terrain specific shader
//	- Multiple noise levels

//TODO: Trees
//	- L-system code
//	- Find tree L-systems
//	- Render whole landscape

//TODO: Camera/UI
//	- Struct used to convert positional data into data necessary for shaders
//	- Provide input for camera controls
//	- UI elements to affect terrain generation

Material emerald =
{
	Vector3(0.0215f, 0.1745f, 0.0215f),
	Vector3(0.07568f, 0.61424f, 0.07568f),
	Vector3(0.633f, 0.727811f, 0.633f),
	256.f
};

Material gold =
{
	Vector3(0.247, 0.199, 0.075),
	Vector3(0.752, 0.606, 0.226),
	Vector3(0.628, 0.556, 0.367),
	25.f
};

Material light_properties =
{
	Vector3(0.2, 0.2, 0.2),
	Vector3(0.9, 0.9, 0.9),
	Vector3(1.0, 1.0, 1.0),
	0.0f
};

Drawable* buffer_trees(int* number_of_trees)
{
	//Generate limited number of models

	//Decide distribution of trees
	
	//Place trees according to distribution
}

//Windows entry point
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR cmd_line, int nCmdShow)
{
	bool platform_ready = initialise_platform(instance);
	bool graphics_ready = initialise_graphics();

	if (platform_ready && graphics_ready)
	{
		int bp_shader = load_shader_program("vshader.glsl", "fshader.glsl");
		int terrain_shader = load_shader_program("vshader.glsl", "terrain_fshader.glsl");
		
		Matrix4x4 model = identity();
		Matrix4x4 view = look_at(Vector3(0.0f, 20.0f, 50.0f), Vector3(0.0f, 0.0f, 0.0f));
		Matrix4x4 projection = perspective(90.0f, get_window_aspect_ratio(), 0.1f, 10.0f);

		set_model_matrix(model);
		set_view_matrix(view);
		set_projection_matrix(projection);

		set_view_origin(Vector3(0.0f, 3.0f, 2.0f));

		set_window_clear_colour(Vector3(0.52, 0.8, 0.92));

		activate_point_light(0);
		set_point_light_position(0, Vector3(0.0f, 0.0f, 2.0f));
		set_point_light_blinn_phong_properties(0, light_properties);
		set_point_light_attenuation_properties(0, 1.0, 0.0014, 0.0007);

		set_max_height(10.0f);

		Drawable terrain = buffer_terrain_mesh(100.0f, 100.0f);
		/*
		int number_of_trees = 0;
		Drawable* trees = buffer_tree_meshes(&number_of_trees);
		*/
		
		//Main loop
		while (!should_window_close()) 
		{
			handle_input();

			//If left mouse button held down and mouse dragged
			//	Rotate view matrix
			//If mouse wheel scrolled

			begin_render();

			use_shader(terrain_shader);

			set_projection_matrix(perspective(90.0f, get_window_aspect_ratio(), 0.1f, 1000.0f));
			set_view_matrix(view);

			set_material(emerald);
			set_model_matrix(identity());
			draw(terrain);

			/*
			for (int i = 0; i < number_of_trees; ++i)
			{
				draw(trees[i]);
			}
			*/
			swap_window_buffers();
		}
		//release_drawable();
		//ReleaseDC(window, window_device_context);
		shutdown_platform();
		return 0;
	}
	else return 1;
}