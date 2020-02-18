#include "Platform.h"
#include "Graphics.h"
#include "Maths.h"
#include "Landscape.h"
#include "l_system.h"
#include "turtle.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_win32.h"

//DOING: 
//	- Fix arcball camera rotation
//	- FPS governing
//		- Timing
//		- Limit frame time
//	- Rendering
//		- Checkboxes for rendering rowans and pines
//		- Checkbox for rendering terrain
//		- Checkbox for rendering as wireframes/polygons
//	- UI module
//		- Program parameters as a struct which UI edits
//		- Keep in consideration that the UI will need to be turned off for release/gameplay mode

//TODO: Platform/Graphics
//	- Internal error handling
//	- Better memory management(?)
//	- glDelete functions

//TODO: Landscape
//	- Parameterise (with UI)
//	- Texture
//	- Retrieve/highlight triangles
//	- Spatial partitioning
//	- Find tree L-systems

//TODO: Volume rendering
//	- Render still uniform density grid
//	- Move densities
//		- Diffusion
//		- Constant wind vector field
//	- Convection of smoke particles
//	- Boundaries in flow caused by trees and hills

//TODO: Camera/UI
//	- UI elements to affect terrain generation
//	- Draw xyz axes

//TODO: Maintenance
//	- Edit interfaces to Platform and Graphics to be:
//		- More consistent
//		- More descriptive (eg. change parameter names in function macros)
//	- Make using OS input easier

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
	Vector3 initial_forward;

	//Two states of arcball code: previous and current, where previous is for the previous mouse drag
	Quaternion arcball_q_previous;
	Quaternion arcball_q_current;
	Matrix4x4 m_current;
	Vector2 drag_origin;
	float movement_sensitivity;

	//Converts given screen coordinates to a vector in the range ([-1, 1], [-1, 1])
	Vector2 screen_coords_to_circle_coords(Vector2 v)
	{
		Vector2 screen_dimensions = { get_window_width(window), get_window_height(window) };
		Vector2 w = Vector2(1.0f, 1.0f) - (v / (0.5f * screen_dimensions));
		if (dot(w, w) > 1.0f) w = normalise(w);
		return w;
	}

	void update_rightward_and_upward_vectors()
	{
		this->rightward = normalise(cross(world_up, -this->forward));
		this->upward = normalise(cross(this->rightward, this->forward));
	}

	void set_position_and_target(Vector3 position, Vector3 target)
	{
		this->arcball_q_current = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);
		this->arcball_q_previous = arcball_q_current;
		this->position = position;
		this->forward = normalise(target - position);
		update_rightward_and_upward_vectors();
	}

	void rotate_by_arcball(Vector2 drag_now)
	{
		drag_now = screen_coords_to_circle_coords(drag_now);
		Quaternion q_0 = compute_great_circle_point(this->drag_origin);
		Quaternion q_1 = compute_great_circle_point(drag_now);

		if (q_0 != q_1)
		{
			Quaternion drag = compute_rotation_between_quaternions(q_0, q_1);
			this->arcball_q_current = drag * this->arcball_q_previous;
			this->m_current = quaternion_to_matrix(conjugate(this->arcball_q_current));
			
			Vector4 forward_4 = Vector4(this->initial_forward, 1.0f);
			forward_4 = this->m_current * forward_4;
			this->forward = forward_4.xyz;
			update_rightward_and_upward_vectors();
		}
	}

	void rotation_start(Vector2 starting_screen_coords)
	{
		this->initial_forward = forward;
		this->drag_origin = screen_coords_to_circle_coords(starting_screen_coords);
	}

	void rotation_end()
	{
		this->arcball_q_previous = this->arcball_q_current;
	}

	void move_forward()
	{
		this->position += movement_sensitivity * forward;
	}

	void move_backward()
	{
		this->position -= movement_sensitivity * forward;
	}
	
	void move_left()
	{
		this->position -= movement_sensitivity * rightward;
	}

	void move_right()
	{
		this->position += movement_sensitivity * rightward;
	}

	void move_up()
	{
		this->position += movement_sensitivity * world_up;
	}

	void move_down()
	{
		this->position -= movement_sensitivity * world_up;
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

//Windows entry point
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR cmd_line, int nCmdShow)
{
	bool platform_ready = initialise_platform(instance);
	bool graphics_ready = initialise_graphics();

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(get_window());
	ImGui_ImplOpenGL3_Init("#version 460");
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	if (platform_ready && graphics_ready)
	{
		int bp_shader = load_shader_program("vshader.glsl", "fshader.glsl");
		int terrain_shader = load_shader_program("vshader.glsl", "terrain_fshader.glsl");
		int terrain_lighting_shader = load_shader_program("vshader.glsl", "fshader.glsl");
		int heightmap_shader = load_shader_program("heightmap_vshader.glsl", "heightmap_fshader.glsl");

		Camera main_view_camera = {};
		main_view_camera.set_position_and_target(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{0.0f, 0.0f, 1.0f});
		main_view_camera.movement_sensitivity = 0.1f;
		set_window_clear_colour(Vector3(0.52, 0.8, 0.92));

		activate_direction_light(0);
		set_direction_light_direction(0, Vector3(0.0f, -1.0f, -1.0f));
		set_direction_light_blinn_phong_properties(0, light_properties);

		Landscape_Data landscape = create_landscape(10.0f, 10.0f, 0.01f, 10);

		bool drawing_as_wireframes = false;
		bool dragging = false;
		//Main loop
		while (!should_window_close()) 
		{
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
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
			
			if (was_mouse_button_pressed(BUTTON_LEFT) && was_mouse_moved() && !dragging)
			{
				dragging = true;
				main_view_camera.rotation_start(get_initial_mouse_position());
			}
			if (was_mouse_moved() && dragging)
			{
				//main_view_camera.rotation_start(get_initial_mouse_position());
				main_view_camera.rotate_by_arcball(get_final_mouse_position());
			}
			if (!was_mouse_button_pressed(BUTTON_LEFT) && dragging)
			{
				main_view_camera.rotation_end();
				dragging = false;
			}

			if (was_key_pressed(KEY_W)) main_view_camera.move_forward();
			if (was_key_pressed(KEY_A)) main_view_camera.move_left();
			if (was_key_pressed(KEY_S)) main_view_camera.move_backward();
			if (was_key_pressed(KEY_D)) main_view_camera.move_right();
			if (was_key_pressed(KEY_Q)) main_view_camera.move_up();
			if (was_key_pressed(KEY_E)) main_view_camera.move_down();

			ImGui::Begin("Camera");
			ImGui::Text("Position");
			ImGui::NewLine();
			ImGui::InputFloat3("##P",&main_view_camera.position[0]);
			ImGui::NewLine();
			ImGui::Text("Forward direction");
			ImGui::NewLine();
			ImGui::InputFloat3("##Q", &main_view_camera.forward[0]);
			ImGui::NewLine();
			ImGui::Text("Movement Sensitivity");
			ImGui::SliderFloat("", &main_view_camera.movement_sensitivity, 0.001f, 1.0f);
			ImGui::End();
			begin_render();
			set_projection_matrix(perspective(50.0f, get_window_aspect_ratio(), 0.1f, 1000.0f));
			set_model_matrix(identity());
			use_shader(terrain_lighting_shader);

			buffer_camera_data_to_gpu(main_view_camera);

			//Draw landscape
			landscape.draw();
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			swap_window_buffers();
		}
		//release_drawable();
		//ReleaseDC(window, window_device_context);
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		shutdown_platform();
		return 0;
	}
	else return 1;
}