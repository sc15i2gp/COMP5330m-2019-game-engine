#include "Platform.h"
#include "Graphics.h"
#include "Maths.h"
#include "Landscape.h"
#include "l_system.h"
#include "turtle.h"
#include "Camera.h"
#include "UI.h"
#include "Timing.h"

//DOING:

//TODO: Platform/Graphics
//	- Internal error handling
//	- Better memory management(?)
//	- glDelete functions
//	- Free resources

//TODO: Landscape
//	- Parameterise (with UI)
//	- Texture
//	- Retrieve/highlight triangles
//	- Spatial partitioning
//	- Fix tree gen

//TODO: Volume rendering
//	- Integrate with scene geometry(?)

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

Drawable buffer_scene_quad()
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

Drawable buffer_raycast_quad()
{
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
	GLuint heightmap_texture = create_texture(heightmap_data_width, heightmap_data_height, heightmap_data, GL_RED);
	dealloc_mem(heightmap_data);
	return heightmap_texture;
}

long int fps_to_mspf(int fps)
{
	float spf = 1.0f / (float)fps;
	float mspf = 1000.0f*spf;
	return (long int)mspf;
}

struct Scalar_Field
{
	float* values;
	int width;
	int height;
	int depth;

	void allocate(int width, int height, int depth)
	{
		this->width = width;
		this->height = height;
		this->depth = depth;
		this->values = (float*)alloc_mem(width * height * depth * sizeof(float));
	}

	void deallocate()
	{
		dealloc_mem(this->values);
	}

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

	void allocate(int width, int height, int depth)
	{
		this->width = width;
		this->height = height;
		this->depth = depth;
		this->values = (Vector3*)alloc_mem(width * height * depth * sizeof(Vector3));
	}

	void deallocate()
	{
		dealloc_mem(this->values);
	}

	Vector3& access(int x, int y, int z)
	{
		return values[z*this->height*this->width + y*this->width + x];
	}
};

void add_scalar_sources(Scalar_Field dst_field, Scalar_Field src_field, float dt)
{
	int width = dst_field.width;
	int height = dst_field.height;
	int depth = dst_field.depth;

	for (int i = 0; i < width * height * depth; ++i)
	{
		dst_field.values[i] += dt * src_field.values[i];
	}
}

void diffuse_scalar_field(Scalar_Field smoke_density_field, Scalar_Field final_smoke_density_field, float diff, float dt)
{
	int width = smoke_density_field.width;
	int height = smoke_density_field.height;
	int depth = smoke_density_field.depth;

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
}

void move_smoke_density_by_vector(Scalar_Field smoke_density_field, Scalar_Field final_smoke_density_field, int f_x, int f_y, int f_z, Vector3 velocity, float dt)
{
	float dx = (float)f_x - dt * velocity.x;
	float dy = (float)f_y - dt * velocity.y;
	float dz = (float)f_z - dt * velocity.z;
	int x_0 = (int)dx;
	int x_1 = x_0 + 1;
	int y_0 = (int)dy;
	int y_1 = y_0 + 1;
	int z_0 = (int)dz;
	int z_1 = z_0 + 1;

	float s_1 = dx - (float)x_0;
	float s_0 = 1.0f - s_1;
	float t_1 = dy - (float)y_0;
	float t_0 = 1.0f - t_1;
	float u_1 = dz - (float)z_0;
	float u_0 = 1.0f - u_1;

	final_smoke_density_field.access(f_x, f_y, f_z) =
		u_0 *
		(
			s_0 * (t_0*smoke_density_field.access(x_0, y_0, z_0) + t_1 * smoke_density_field.access(x_0, y_1, z_0)) +
			s_1 * (t_0*smoke_density_field.access(x_1, y_0, z_0) + t_1 * smoke_density_field.access(x_1, y_1, z_0))
			) +
		u_1 *
		(
			s_0 * (t_0*smoke_density_field.access(x_0, y_0, z_1) + t_1 * smoke_density_field.access(x_0, y_1, z_1)) +
			s_1 * (t_0*smoke_density_field.access(x_1, y_0, z_1) + t_1 * smoke_density_field.access(x_1, y_1, z_1))
			);
}

void move_smoke_density(Scalar_Field smoke_density_field, Scalar_Field final_smoke_density_field, Vector3_Field velocity_field, float dt)
{
	int width = smoke_density_field.width;
	int height = smoke_density_field.height;
	int depth = smoke_density_field.depth;

	for (int z = 1; z < depth - 1; ++z)
	{
		for (int y = 1; y < height - 1; ++y)
		{
			for (int x = 1; x < width - 1; ++x)
			{
				Vector3 velocity = velocity_field.access(x, y, z);
				move_smoke_density_by_vector(smoke_density_field, final_smoke_density_field, x, y, z, velocity, dt);
			}
		}
	}
}

void convect_temperature(Scalar_Field temperature_field, Scalar_Field final_temperature_field, float dt)
{
	int width = temperature_field.width;
	int height = temperature_field.height;
	int depth = temperature_field.depth;

	for (int z = 1; z < depth - 1; ++z)
	{
		for (int y = 1; y < depth - 1; ++y)
		{
			for (int x = 1; x < depth - 1; ++x)
			{
				final_temperature_field.access(x, y, z) = temperature_field.access(x, y, z) 
					+ dt * (temperature_field.access(x, y - 1, z) - temperature_field.access(x, y, z));
			}
		}
	}
}

void copy_scalar_field_values(Scalar_Field src, Scalar_Field dst)
{
	int width = src.width;
	int height = src.height;
	int depth = src.depth;

	for (int i = 0; i < width * height * depth; ++i) dst.values[i] = src.values[i];
}

Vector3 compute_updraft_from_temperature(float temperature)
{
	//Linear between temp of 0 and some max temp
	float max_temperature = 1500.0f;
	Vector3 max_up(0.0f, 100.0f, 0.0f);
	float f = temperature/max_temperature;
	return f * max_up;
}

void convect_smoke_density(Scalar_Field smoke_density_field, Scalar_Field final_smoke_density_field, Scalar_Field temperature_field, float dt)
{
	for (int z = 1; z < smoke_density_field.depth - 1; ++z)
	{
		for (int y = 1; y < smoke_density_field.height - 1; ++y)
		{
			for (int x = 1; x < smoke_density_field.width - 1; ++x)
			{
				Vector3 velocity = compute_updraft_from_temperature(temperature_field.access(x, y, z));
				move_smoke_density_by_vector(smoke_density_field, final_smoke_density_field, x, y, z, velocity, dt);
			}
		}
	}
}

void swap_field_ptrs(Scalar_Field& f_0, Scalar_Field& f_1)
{
	float* temp = f_0.values;
	f_0.values = f_1.values;
	f_1.values = temp;
}

//TODO: Either rewrite or document this function better, it sucks
void progress_smoke_simulation(Scalar_Field smoke_density_field, Scalar_Field smoke_source_field, Scalar_Field temperature_field, 
	Vector3_Field wind_velocity_field, float diff, float dt)
{
	TIME_FUNCTION;
	int width = smoke_density_field.width;
	int height = smoke_density_field.height;
	int depth = smoke_density_field.depth;

	Scalar_Field final_smoke_density_field = smoke_density_field;
	final_smoke_density_field.values = (float*)alloc_mem(width * height * depth * sizeof(float));

	add_scalar_sources(smoke_density_field, smoke_source_field, dt);
	diffuse_scalar_field(smoke_density_field, final_smoke_density_field, diff, dt);

	swap_field_ptrs(smoke_density_field, final_smoke_density_field);

	move_smoke_density(smoke_density_field, final_smoke_density_field, wind_velocity_field, dt);

	swap_field_ptrs(smoke_density_field, final_smoke_density_field);

	convect_smoke_density(smoke_density_field, final_smoke_density_field, temperature_field, dt);

	copy_scalar_field_values(final_smoke_density_field, smoke_density_field);

	dealloc_mem(final_smoke_density_field.values);
}


void progress_temperature_simulation(Scalar_Field temperature_field, Scalar_Field temperature_source_field, float diff, float dt)
{
	float* src_temperature_values = temperature_field.values;
	float* dst_temperature_values = (float*)alloc_mem(temperature_field.width * temperature_field.height * temperature_field.depth * sizeof(float));

	Scalar_Field final_temperature_field = temperature_field;
	final_temperature_field.values = dst_temperature_values;

	add_scalar_sources(temperature_field, temperature_source_field, dt);

	diffuse_scalar_field(temperature_field, final_temperature_field, diff, dt);

	swap_field_ptrs(temperature_field, final_temperature_field);

	convect_temperature(temperature_field, final_temperature_field, dt); //Just moves temperature values upwards

	temperature_field.values = src_temperature_values;

	final_temperature_field.values = dst_temperature_values;
	dealloc_mem(final_temperature_field.values);
}

void buffer_scalar_field_xz_slice_to_texture(Scalar_Field field, int y, GLuint texture)
{
	float* values = (float*)alloc_mem(field.width * field.depth * sizeof(float));
	for (int z = 0; z < field.depth; ++z)
	{
		for (int x = 0; x < field.width; ++x)
		{
			values[z*field.width + x] = field.access(x, y, z);
		}
	}
	dealloc_mem(values);
	buffer_texture_data(texture, field.width, field.depth, values, GL_RED);
}

void buffer_scalar_field_xy_slice_to_texture(Scalar_Field field, int z, GLuint texture)
{
	float* values = (float*)alloc_mem(field.width * field.depth * sizeof(float));
	for (int y = 0; y < field.height; ++y)
	{
		for (int x = 0; x < field.width; ++x)
		{
			values[y*field.width + x] = field.access(x, y, z);
		}
	}
	dealloc_mem(values);
	buffer_texture_data(texture, field.width, field.height, values, GL_RED);
}

//Buffers a horizontal slice of the given scalar field to a texture
GLuint buffer_scalar_field_xz_slice_to_texture(Scalar_Field field, int y)
{
	GLuint texture = create_texture(field.width, field.depth, NULL, GL_RED);
	buffer_scalar_field_xz_slice_to_texture(field, y, texture);
	return texture;
}

GLuint buffer_scalar_field_xy_slice_to_texture(Scalar_Field field, int z)
{
	GLuint texture = create_texture(field.width, field.depth, NULL, GL_RED);
	buffer_scalar_field_xy_slice_to_texture(field, z, texture);
	return texture;
}

class Smoke_Simulation
{
public:
	void allocate_fields(int field_width, int field_height, int field_depth);
	void deallocate_fields();
	void set_wind_vector(Vector3 wind_vector);
	void add_smoke_source(int x, int y, int z, float s);
	void add_temperature_source(int x, int y, int z, float t);

	void progress_simulation(float smoke_diff, float temp_diff, float dt);

	float* get_smoke_density_values();
	float* get_temperature_values();

private:
	Scalar_Field smoke_density_field;
	Scalar_Field smoke_density_source_field;
	Scalar_Field temperature_field;
	Scalar_Field temperature_source_field;
	Vector3_Field wind_field;
};

void Smoke_Simulation::allocate_fields(int width, int height, int depth)
{
	this->smoke_density_field.allocate(width, height, depth);
	this->smoke_density_source_field.allocate(width, height, depth);
	this->temperature_field.allocate(width, height, depth);
	this->temperature_source_field.allocate(width, height, depth);
	this->wind_field.allocate(width, height, depth);
}

void Smoke_Simulation::deallocate_fields()
{
	this->smoke_density_field.deallocate();
	this->smoke_density_source_field.deallocate();
}

void Smoke_Simulation::set_wind_vector(Vector3 wind_vector)
{
	for (int i = 0; i < this->wind_field.width * this->wind_field.height * this->wind_field.depth; ++i) this->wind_field.values[i] = wind_vector;
}

void Smoke_Simulation::add_smoke_source(int x, int y, int z, float s)
{
	this->smoke_density_source_field.access(x, y, z) = s;
}

void Smoke_Simulation::add_temperature_source(int x, int y, int z, float t)
{
	this->temperature_source_field.access(x, y, z) = t;
}

void Smoke_Simulation::progress_simulation(float smoke_diff, float temp_diff, float dt)
{
	progress_temperature_simulation(this->temperature_field, this->temperature_source_field, smoke_diff, dt);
	progress_smoke_simulation(this->smoke_density_field, this->smoke_density_source_field, this->temperature_field, this->wind_field, temp_diff, dt);
}

float* Smoke_Simulation::get_smoke_density_values()
{
	return this->smoke_density_field.values;
}

float* Smoke_Simulation::get_temperature_values()
{
	return this->temperature_field.values;
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
		int quad_texture_shader = load_shader_program("heightmap_vshader.glsl", "heightmap_fshader.glsl");
		
		int scene_framebuffer = alloc_framebuffer(get_window_width(), get_window_height());
		int smoke_framebuffer = alloc_framebuffer(get_window_width(), get_window_height());
		
		OutputDebugStringf("Scene Framebuffer: %d\n", scene_framebuffer);
		OutputDebugStringf("Smoke framebuffer: %d\n", smoke_framebuffer);
		Drawable scene_quad = buffer_scene_quad();
		Drawable smoke_quad = buffer_raycast_quad();
		
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
		bool render_density_field_slice = false;
		float fov = 90.0f;
		UI_Parameters ui_parameters = initialise_ui_parameter_pointers(&landscape, &main_view_camera, &fps, &render_wireframes, &render_depth_buffer, 
			&render_density_field_slice, &fov);

		set_shader_sampler_uniform(framebuffer_composite_shader, "scene_texture", 1);
		set_shader_sampler_uniform(framebuffer_composite_shader, "smoke_texture", 0);
		set_shader_sampler_uniform(smoke_shader, "density_field", 0);
		set_shader_sampler_uniform(smoke_shader, "temperature_field", 1);

		set_max_density(10.0f);
		
		Vector3 wind_vector = Vector3(1.0f, 1.0f, 1.0f);

		int smoke_field_width = 32;
		int smoke_field_height = 32;
		int smoke_field_depth = 32;
		Smoke_Simulation smoke_simulation;
		smoke_simulation.allocate_fields(smoke_field_width, smoke_field_height, smoke_field_depth);
		smoke_simulation.add_smoke_source(2, 2, 2, 1000.0f);
		smoke_simulation.add_temperature_source(2, 2, 2, 3000.0f);
		smoke_simulation.set_wind_vector(wind_vector);

		int density_field_texture = create_volume_texture(smoke_field_width, smoke_field_height, smoke_field_depth);
		int temperature_field_texture = create_volume_texture(smoke_field_width, smoke_field_height, smoke_field_depth);
		set_world_smoke_volume_coefficient(3.2f);
		set_max_temperature(1000.0f);
		buffer_volume_data(density_field_texture, smoke_field_width, smoke_field_height, smoke_field_depth, smoke_simulation.get_smoke_density_values());
		buffer_volume_data(temperature_field_texture, smoke_field_width, smoke_field_height, smoke_field_depth, smoke_simulation.get_temperature_values());

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
			smoke_simulation.progress_simulation(1.0f, 0.5f, mspf / 1000.0f);
			
			buffer_volume_data(density_field_texture, smoke_field_width, smoke_field_height, smoke_field_depth, smoke_simulation.get_smoke_density_values());
			buffer_volume_data(temperature_field_texture, smoke_field_width, smoke_field_height, smoke_field_depth, smoke_simulation.get_temperature_values());
			
			//RENDERING
			//Set global rendering parameters
			if (render_wireframes) draw_as_wireframes();
			else draw_as_polygons();

			if (!render_density_field_slice)
			{
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
				use_volume_texture(temperature_field_texture, 1);
				set_model_matrix(identity());
				draw(smoke_quad);

				//Composite framebuffers and render to window
				use_framebuffer(DEFAULT_FRAMEBUFFER);
				begin_render();

				use_shader(framebuffer_composite_shader);
				use_framebuffer_texture(scene_framebuffer, 1);
				use_framebuffer_texture(smoke_framebuffer, 0);

				draw(scene_quad);
				use_texture(0, 0);
				use_texture(0, 1);
			}
			else
			{
				begin_render();

				use_shader(quad_texture_shader);
				//use_texture(temp_field_slice_texture);

				draw(scene_quad);
				use_texture(0);

				render_ui();
			}
			
			swap_window_buffers();

			stop_timer(&t);
			long int frame_time = elapsed_time(&t);
			OutputDebugStringf("Elapsed time: %li\n", frame_time);
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