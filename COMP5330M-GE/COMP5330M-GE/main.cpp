#include "Platform.h"
#include "Graphics.h"
#include "Maths.h"
#include "Landscape.h"
#include "l_system.h"
#include "turtle.h"
#include "Camera.h"
#include "UI.h"

//DOING:
//	- Smoke movement due to velocity field
//	- Smoke movement due to temperature

//TODO: Platform/Graphics
//	- Internal error handling
//	- Better memory management(?)
//	- glDelete functions
//	- Create separate 2d texture sub buffer function
//	- Free resources

//TODO: Landscape
//	- Parameterise (with UI)
//	- Texture
//	- Retrieve/highlight triangles
//	- Spatial partitioning
//	- Find tree L-systems

//TODO: Volume rendering
//	- Smoke colour due to temperature
//	- Integrate with scene geometry

//TODO: Camera/UI
//	- Draw xyz axes
//	- Fix arcball camera rotation
//	- Make it so that if a mouse click is on a UI element, it doesn't rotate the camera
//	- Better starting dimensions (not too small or big)
//	- Better integration of render surface and UI
//		- Solves problems such as the camera rotating while using a slider without nasty workarounds

//TODO: Maintenance
//	- Edit interfaces to Platform and Graphics to be:
//		- More consistent
//		- More descriptive (eg. change parameter names in function macros)
//	- Make using OS input easier
//	- Directory structure to group files/project contributions

//TODO: Profiling
//	- Time functions

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

Drawable buffer_quad()
{
	//Create quad mesh
	Mesh_vertex quad_vertices[4] = {};
	//Top left
	quad_vertices[0] =
	{
		{-1.0f, -1.0f, 0.0f}, //Position
		{}, //Normal
		{0.0f, 0.0f}  //Texture coords
	};
	//Bottom left
	quad_vertices[1] =
	{
		{-1.0f, 1.0f, 0.0f}, //Position
		{}, //Normal
		{0.0f, 1.0f}  //Texture coords
	};
	//Bottom right
	quad_vertices[2] =
	{
		{1.0f, 1.0f, 0.0f}, //Position
		{}, //Normal
		{1.0f, 1.0f}  //Texture coords
	};
	//Top right
	quad_vertices[3] =
	{
		{1.0f, -1.0f, 0.0f}, //Position
		{}, //Normal
		{1.0f, 0.0f}  //Texture coords
	};
	GLuint quad_indices[] = { 0, 1, 2, 2, 3, 0 };
	Drawable quad = buffer_mesh(quad_vertices, 4, quad_indices, 6);
	return quad;
}

GLuint buffer_heightmap()
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
	GLuint heightmap_texture = buffer_texture(heightmap_data_width, heightmap_data_height, heightmap_data, GL_RED);
	dealloc_mem(heightmap_data);
	return heightmap_texture;
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

struct Scalar_Field
{
	float* values;
	int width;
	int height;
	int depth;

	float& access(int x, int y, int z)
	{
		return values[z*this->height*this->width + y * this->width + x];
	}
};

struct Vector3_Field
{
	Vector3* values;
	int width;
	int height;
	int depth;

	Vector3& access(int x, int y, int z)
	{
		return values[z*this->height*this->width + y*this->width + x];
	}
};

void progress_smoke_simulation(Scalar_Field smoke_density_field, Scalar_Field smoke_source_field, float diff, float dt)
{
	int width = smoke_density_field.width;
	int height = smoke_density_field.height;
	int depth = smoke_density_field.depth;

	Scalar_Field final_smoke_density_field = smoke_density_field;
	final_smoke_density_field.values = (float*)alloc_mem(width * height * depth * sizeof(float));

	for (int i = 0; i < width * height * depth; ++i)
	{
		smoke_density_field.values[i] += dt * smoke_source_field.values[i];
		final_smoke_density_field.values[i] = smoke_density_field.values[i];
	}

	//Diffusion
	float diffusion_rate = diff * dt;
	for (int i = 0; i < 20; ++i)
	{
		for (int z = 1; z < depth - 1; ++z)
		{
			for (int y = 1; y < height - 1; ++y)
			{
				for (int x = 1; x < width - 1; ++x)
				{
					final_smoke_density_field.access(x, y, z) = (smoke_density_field.access(x, y, z)
						+ diffusion_rate * (final_smoke_density_field.access(x - 1, y, z) + final_smoke_density_field.access(x + 1, y, z)
							+ final_smoke_density_field.access(x, y - 1, z) + final_smoke_density_field.access(x, y + 1, z)
							+ final_smoke_density_field.access(x, y, z - 1) + final_smoke_density_field.access(x, y, z + 1)))
						/ (1 + 6 * diffusion_rate);
				}
			}
		}
	}
	float max_density = 5.0f;
	for (int z = 0; z < depth; ++z)
	{
		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				float value = final_smoke_density_field.access(x, y, z);
				smoke_density_field.access(x, y, z) = (value < max_density) ? value : max_density;
			}
		}
	}
	dealloc_mem(final_smoke_density_field.values);
}

//Windows entry point
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR cmd_line, int nCmdShow)
{
	bool platform_ready = initialise_platform(instance);
	bool graphics_ready = initialise_graphics();
	bool ui_ready = initialise_ui();

	if (platform_ready && graphics_ready)
	{
		int terrain_lighting_shader = load_shader_program("vshader.glsl", "fshader.glsl");
		int depth_shader = load_shader_program("vshader.glsl", "depth_fshader.glsl");
		int framebuffer_composite_shader = load_shader_program("framebuffer_vshader.glsl", "framebuffer_fshader.glsl");
		int smoke_shader = load_shader_program("smoke_vshader.glsl", "smoke_fshader.glsl");
		
		int scene_framebuffer = alloc_framebuffer(get_window_width(), get_window_height());
		int smoke_framebuffer = alloc_framebuffer(get_window_width(), get_window_height());
		
		OutputDebugStringf("Scene Framebuffer: %d\n", scene_framebuffer);
		OutputDebugStringf("Smoke framebuffer: %d\n", smoke_framebuffer);
		Drawable scene_quad = buffer_quad();
		
		Camera main_view_camera = {};
		main_view_camera.set_position_and_target(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{0.0f, 0.0f, 1.0f});
		main_view_camera.movement_sensitivity = 0.1f;
		Vector3 sky_colour(0.52, 0.8, 0.92);
		Vector3 black = Vector3();

		activate_direction_light(0);
		set_direction_light_direction(0, Vector3(0.0f, -1.0f, -1.0f));
		set_direction_light_blinn_phong_properties(0, light_properties);

		float landscape_width = 10.0f;
		float landscape_length = 10.0f;
		Landscape_Data landscape = create_landscape(landscape_width, landscape_length, 0.01f, 10);

		int field_width = (int)landscape_width;
		int field_height = 10;
		int field_depth = (int)landscape_length;
		bool dragging = false;
		int fps = 60;
		bool render_wireframes = false;
		bool render_depth_buffer = false;
		float fov = 90.0f;
		UI_Parameters ui_parameters = initialise_ui_parameter_pointers(&landscape, &main_view_camera, &fps, &render_wireframes, &render_depth_buffer, &fov);

		set_shader_sampler_uniform(framebuffer_composite_shader, "scene_texture", 1);
		set_shader_sampler_uniform(framebuffer_composite_shader, "smoke_texture", 0);
		set_shader_sampler_uniform(smoke_shader, "density_field", 0);

		Scalar_Field smoke_density_field;
		smoke_density_field.values = (float*)alloc_mem(field_width * field_height * field_depth * sizeof(float));
		smoke_density_field.width = field_width;
		smoke_density_field.height = field_height;
		smoke_density_field.depth = field_depth;
		for (int z = 1; z < field_depth - 1; ++z)
		{
			for (int y = 1; y < field_height - 1; ++y)
			{
				for (int x = 1; x < field_width - 1; ++x)
				{
					smoke_density_field.access(x, y, z) = (x == 1) ? 0.5f : 0.0f;
				}
			} 
		}
		Scalar_Field smoke_source_field = smoke_density_field;
		smoke_source_field.values = (float*)alloc_mem(field_width * field_height * field_depth * sizeof(float));
		smoke_source_field.access(2, 2, 2) = 1.0f;

		int density_field_texture = create_volume_texture(field_width, field_height, field_depth);
		buffer_volume_data(density_field_texture, field_width, field_height, field_depth, smoke_density_field.values);
		
		timer t;
		//Main loop
		while (!should_window_close()) 
		{
			start_timer(&t);
			long int mspf = fps_to_mspf(fps);

			//INPUT + UI
			handle_input();
			handle_ui(ui_parameters);
			
			if (was_mouse_button_pressed(BUTTON_LEFT) && was_mouse_moved() && !dragging && !ImGui::IsAnyItemActive())
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

			if (was_window_resized()) resize_framebuffers(get_window_width(), get_window_height());

			//UPDATE
			progress_smoke_simulation(smoke_density_field, smoke_source_field, 0.01f, mspf/1000.0f);
			
			buffer_volume_data(density_field_texture, field_width, field_height, field_depth, smoke_density_field.values);
			
			//RENDERING
			//Set global rendering parameters
			if (render_wireframes) draw_as_wireframes();
			else draw_as_polygons();

			//Render scene into scene buffer
			use_framebuffer(scene_framebuffer);
			set_window_clear_colour(sky_colour);
			begin_render();
			
			
			Matrix4x4 projection = perspective(fov, get_window_aspect_ratio(), 0.1f, 10.0f);
			set_projection_matrix(projection);

			set_model_matrix(identity());
			if (!render_depth_buffer) use_shader(terrain_lighting_shader);
			else use_shader(depth_shader);

			buffer_camera_data_to_gpu(main_view_camera);
			
			//Draw landscape
			landscape.draw();
			render_ui();
			
			//Render smoke
			use_framebuffer(smoke_framebuffer);
			set_window_clear_colour(black);
			begin_render();
			use_shader(smoke_shader);
			use_volume_texture(density_field_texture, 0);
			set_model_matrix(identity());
			draw(scene_quad);
			
			//Composite framebuffers and render to window
			use_framebuffer(DEFAULT_FRAMEBUFFER);
			begin_render();
			
			use_shader(framebuffer_composite_shader);
			use_framebuffer_texture(scene_framebuffer, 1);
			use_framebuffer_texture(smoke_framebuffer, 0);
			
			draw(scene_quad);
			use_texture(0, 0);
			use_texture(0, 1);
			
			
			swap_window_buffers();

			stop_timer(&t);
			long int frame_time = elapsed_time(&t);
			if (frame_time < mspf)
			{
				DWORD sleep_time = mspf - frame_time;
				Sleep(sleep_time);
			}
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