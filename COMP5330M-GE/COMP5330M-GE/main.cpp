#include "Platform.h"
#include "Graphics.h"
#include "Maths.h"
#include "Landscape.h"
#include "l_system.h"
#include "turtle.h"

//DOING: 
//	- UI to change terrain
//	- Add trees

//TODO: Platform
//	- Internal error handling
//	- Timing
//	- Better memory management(?)

//TODO: Terrain
//	- Parameterise (with UI)
//	- Texture
//	- Retrieve/highlight triangles
//	- Only render certain range of terrain

//TODO: Trees
//	- L-system code
//	- Find tree L-systems
//	- Render whole landscape

//TODO: Camera/UI
//	- UI elements to affect terrain generation
//	- Fix arcball camera rotation

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

Material wood =
{
	Vector3(0.76, 0.6, 0.42),
	Vector3(0.76, 0.6, 0.42),
	Vector3(0.76, 0.6, 0.42),
	12.0
};

Material light_properties =
{
	Vector3(0.2, 0.2, 0.2),
	Vector3(0.5, 0.5, 0.5),
	Vector3(0.6, 0.6, 0.6),
	0.0f
};

Vector3 world_up = { 0.0f, 1.0f, 0.0f };
struct Camera
{
	Vector3 position;
	Vector3 forward;
	Vector3 upward;
	Vector3 rightward;
	Quaternion arcball_rotation;

	void update_rightward_and_upward_vectors()
	{
		this->rightward = normalise(cross(world_up, this->position));
		this->upward = normalise(cross(this->rightward, this->forward));
	}

	void set_position_and_target(Vector3 position, Vector3 target)
	{
		this->position = position;
		this->forward = normalise(target - position);
		this->arcball_rotation = Quaternion(Vector3(0.0f, 1.0f, 0.0f), 0.0f);
		update_rightward_and_upward_vectors();
	}

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
			/*
			Quaternion drag = compute_rotation_between_quaternions(q_0, q_1);
			drag = compute_rotation_between_quaternions(drag, this->arcball_rotation);
			Matrix3x3 drag_rotation = quaternion_to_matrix(drag);
			*/
			Matrix3x3 drag_rotation = compute_rotation_matrix_between_quaternions(q_0, q_1);
			Matrix3x3 m = drag_rotation;
			
			/*
			OutputDebugStringf("M: %f %f %f\n", m[0].x, m[1].x, m[2].x);
			OutputDebugStringf("M: %f %f %f\n", m[0].y, m[1].y, m[2].y);
			OutputDebugStringf("M: %f %f %f\n", m[0].z, m[1].z, m[2].z);
			OutputDebugStringf("%f %f %f %f\n", this->arcball_rotation.w, this->arcball_rotation.x, this->arcball_rotation.y, this->arcball_rotation.z);
			*/
			this->forward *= drag_rotation;
			this->upward *= drag_rotation;
			this->rightward *= drag_rotation;

		}
	}

	void move_forward()
	{
		this->position += 0.1f * forward;
	}

	void move_backward()
	{
		this->position -= 0.1f * forward;
	}
	
	void move_left()
	{
		this->position -= 0.1f * rightward;
	}

	void move_right()
	{
		this->position += 0.1f * rightward;
	}

	void move_up()
	{
		this->position += 0.1f * world_up;
	}

	void move_down()
	{
		this->position -= 0.1f * world_up;
	}
};

void buffer_camera_data_to_gpu(Camera c)
{
	//Compute lookat matrix

	//Matrix4x4 view = look_at(c.position, c.position + c.forward);
	Matrix4x4 view = look_at(c.position, -c.forward, c.rightward, c.upward);

	//Buffer lookat matrix to shader
	set_view_matrix(view);

	//Buffer view position to shader
	set_view_origin(c.position);
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

bool show_tree = true;
//Windows entry point
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR cmd_line, int nCmdShow)
{
	bool platform_ready = initialise_platform(instance);
	bool graphics_ready = initialise_graphics();

	if (platform_ready && graphics_ready)
	{
		int bp_shader = load_shader_program("vshader.glsl", "fshader.glsl");
		int terrain_shader = load_shader_program("vshader.glsl", "terrain_fshader.glsl");
		int terrain_lighting_shader = load_shader_program("vshader.glsl", "fshader.glsl");
		int heightmap_shader = load_shader_program("heightmap_vshader.glsl", "heightmap_fshader.glsl");

		Camera main_view_camera = {};
		main_view_camera.set_position_and_target(Vector3{ 0.0f, 0.0f, -1.0f }, Vector3{});

		set_window_clear_colour(Vector3(0.52, 0.8, 0.92));

		activate_direction_light(0);
		set_direction_light_direction(0, Vector3(0.0f, -1.0f, -1.0f));
		set_direction_light_blinn_phong_properties(0, light_properties);

		Terrain terrain = create_terrain(10.0f, 10.0f, 0.01f, 0.1f);
		//set_max_height(terrain.max_height);
		OutputDebugStringf("Max height = %f\n", terrain.max_height);
		bool drawing_as_wireframes = false;

		GLuint heightmap_texture;
		Drawable heightmap = buffer_heightmap_to_textured_quad(&heightmap_texture);
		OutputDebugStringf("Texture: %d\n", heightmap_texture);

		l_system example_l_system = {};
		add_production(&example_l_system, "<A>", "BC");
		add_production(&example_l_system, "<C>", "DA");

		char str[2048] = {};
		strcpy(str, "A");

		tree_mesh_group tree = {};
		tree.branch_mesh.face_indices = (GLuint*)alloc_mem(2048 * sizeof(GLuint));
		tree.branch_mesh.vertices = (Mesh_vertex*)alloc_mem(512 * sizeof(Mesh_vertex));
		tree.leaf_mesh.face_indices = (GLuint*)alloc_mem(2048 * sizeof(GLuint));
		tree.leaf_mesh.vertices = (Mesh_vertex*)alloc_mem(512 * sizeof(Mesh_vertex));
		
		run_turtle("F(0.5, 0.2)[+(45.0)F(0.5, 0.1)@(0.3)][-(45.0)F(0.5, 0.1)@(0.3)]", &tree);
		//run_turtle("F(0.5)@(0.5)", &tree);
		OutputDebugStringf("Tree mesh: %d %d\n", tree.leaf_mesh.number_of_vertices, tree.leaf_mesh.number_of_indices);
		Drawable tree_branches = buffer_mesh(tree.branch_mesh);
		Drawable tree_leaves = buffer_mesh(tree.leaf_mesh);

		//Main loop
		while (!should_window_close()) 
		{
			handle_input();

			if (was_key_pressed(KEY_RETURN))
			{
				if (drawing_as_wireframes)
				{
					draw_as_polygons();
					drawing_as_wireframes = false;
				}
				else
				{
					draw_as_wireframes();
					drawing_as_wireframes = true;
				}
			}
			
			if (was_mouse_button_pressed(BUTTON_LEFT) && was_mouse_moved()) main_view_camera.rotate_by_arcball(get_initial_mouse_position(), get_final_mouse_position());

			if (was_key_pressed(KEY_W)) main_view_camera.move_forward();
			if (was_key_pressed(KEY_A)) main_view_camera.move_left();
			if (was_key_pressed(KEY_S)) main_view_camera.move_backward();
			if (was_key_pressed(KEY_D)) main_view_camera.move_right();
			if (was_key_pressed(KEY_Q)) main_view_camera.move_up();
			if (was_key_pressed(KEY_E)) main_view_camera.move_down();
			if (was_key_pressed(KEY_SHIFT)) show_tree = !show_tree;

			begin_render();
			set_projection_matrix(perspective(90.0f, get_window_aspect_ratio(), 0.1f, 1000.0f));
			set_model_matrix(identity());

			if (show_tree)
			{
				use_shader(terrain_lighting_shader);

				set_view_matrix(identity());

				buffer_camera_data_to_gpu(main_view_camera);

				set_material(wood);
				draw(tree_branches);
				set_material(emerald);
				draw(tree_leaves);
			}
			else
			{
				use_shader(terrain_lighting_shader);
				
				buffer_camera_data_to_gpu(main_view_camera);

				set_material(emerald);
				draw(terrain.graphical_data);
			}

			swap_window_buffers();
		}
		//release_drawable();
		//ReleaseDC(window, window_device_context);
		shutdown_platform();
		return 0;
	}
	else return 1;
}