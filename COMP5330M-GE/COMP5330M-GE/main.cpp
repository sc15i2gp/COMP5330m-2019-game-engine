#include "Platform.h"
#include "Graphics.h"
#include "Maths.h"
#include "Landscape.h"

//DOING: UI + Camera
//	- It works, I just now need to make it easier to use, such as only being able to move forward relative to the camera
//		but move left, right, up and down relative to the whole world
//	- Fix terrain gen. For some reason, some of the vertices have 0 height and will sometimes stretch downwards a great length.
//		My guess is that some vertex data isn't being initialised properly, or the perlin noise function is being sampled beyond its grid bounds

//TODO: Platform
//	- REMOVE try-catch from alloc mem
//	- Input events
//	- API
//	- Internal error handling
//	- Timing
//	- Better memory management(?)

//TODO: Terrain
//	- Parameterise (with UI)
//	- Terrain specific shader
//	- Multiple noise levels

//TODO: Trees
//	- L-system code
//	- Find tree L-systems
//	- Render whole landscape

//TODO: Camera/UI
//	- UI elements to affect terrain generation

//TODO: Limit FPS

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

struct Camera
{
	Vector3 position;
	Vector3 forward;
	Vector3 upward;
	Vector3 rightward;

	void rotate_by_arcball(Vector2 initial_mouse_position, Vector2 final_mouse_position)
	{
		Vector2 screen_dimensions = { get_window_width(window), get_window_height(window) };

		initial_mouse_position.x = 1.0f - (initial_mouse_position.x / (0.5f * screen_dimensions.x));
		initial_mouse_position.y = 1.0f - (initial_mouse_position.y / (0.5f * screen_dimensions.y));
		final_mouse_position.x = 1.0f - (final_mouse_position.x / (0.5f * screen_dimensions.x));
		final_mouse_position.y = 1.0f - (final_mouse_position.y / (0.5f * screen_dimensions.y));

		Quaternion q_0 = compute_great_circle_point(initial_mouse_position);
		Quaternion q_1 = compute_great_circle_point(final_mouse_position);

		if (q_0 != q_1)
		{
			Matrix3x3 drag_rotation = compute_rotation_between_quaternions(q_0, q_1);
			this->forward *= drag_rotation;
			this->upward *= drag_rotation;
			this->rightward *= drag_rotation;
		}
	}

	void move_forward()
	{
		this->position += forward;
	}

	void move_backward()
	{
		this->position -= forward;
	}
	
	void move_left()
	{
		this->position -= rightward;
	}

	void move_right()
	{
		this->position += rightward;
	}

	void move_up()
	{
		this->position += upward;
	}

	void move_down()
	{
		this->position -= upward;
	}
};

void buffer_camera_data_to_gpu(Camera c)
{
	//Compute lookat matrix
	Matrix4x4 view = look_at(c.position, c.position + c.forward);

	//Buffer lookat matrix to shader
	set_view_matrix(view);

	//Buffer view position to shader
	set_view_origin(c.position);
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

		Camera main_view_camera = {};
		main_view_camera.position = { 0.0f, 20.0f, 50.0f };
		main_view_camera.forward = normalise(-main_view_camera.position); //Set initial camera target to O
		main_view_camera.rightward = normalise(cross(Vector3(0.0f, 1.0f, 0.0f), main_view_camera.position));
		main_view_camera.upward = normalise(cross(main_view_camera.rightward, main_view_camera.forward));

		set_window_clear_colour(Vector3(0.52, 0.8, 0.92));

		activate_point_light(0);
		set_point_light_position(0, Vector3(0.0f, 0.0f, 2.0f));
		set_point_light_blinn_phong_properties(0, light_properties);
		set_point_light_attenuation_properties(0, 1.0, 0.0014, 0.0007);

		Terrain terrain = create_terrain(100.0f, 100.0f);
		set_max_height(terrain.max_height);
		/*
		int number_of_trees = 0;
		Drawable* trees = buffer_tree_meshes(&number_of_trees);
		*/
		
		//Main loop
		while (!should_window_close()) 
		{
			handle_input();

			
			if (was_mouse_button_pressed(BUTTON_LEFT) && was_mouse_moved()) main_view_camera.rotate_by_arcball(get_initial_mouse_position(), get_final_mouse_position());

			if (was_key_pressed(KEY_W)) main_view_camera.move_forward();
			if (was_key_pressed(KEY_A)) main_view_camera.move_left();
			if (was_key_pressed(KEY_S)) main_view_camera.move_backward();
			if (was_key_pressed(KEY_D)) main_view_camera.move_right();
			if (was_key_pressed(KEY_Q)) main_view_camera.move_up();
			if (was_key_pressed(KEY_E)) main_view_camera.move_down();

			begin_render();

			use_shader(terrain_shader);

			set_projection_matrix(perspective(90.0f, get_window_aspect_ratio(), 0.1f, 1000.0f));
			
			buffer_camera_data_to_gpu(main_view_camera);

			set_material(emerald);
			set_model_matrix(identity());
			draw(terrain.graphical_data);

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