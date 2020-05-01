#include "Platform.h"
#include "Graphics.h"
#include "Maths.h"
#include "Landscape.h"
#include "l_system.h"
#include "turtle.h"
#include "Camera.h"
#include "UI.h"
#include "RigidBody.h"
#include "Particles.h"
#include <thread>

//DOING:

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
//	- Draw xyz axes
//	- Fix arcball camera rotation
//	- Make it so that if a mouse click is on a UI element, it doesn't rotate the camera
//	- Better starting dimensions (not too small or big)

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

//Render 4*4 perlin noise grid on 64*64 texture
Drawable buffer_heightmap_to_textured_quad(GLuint* heightmap_texture)
{
	Perlin_Noise_Function perlin_noise = generate_noise_function();
	GLuint heightmap_data_width = 1024;
	GLuint heightmap_data_height = 1024;

	//Buffer texture data
	GLfloat* heightmap_data = (GLfloat*)alloc_mem(heightmap_data_width * heightmap_data_height * sizeof(GLfloat));
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
	float mspf = 1000.0f * spf;
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
	bool platform_ready = initialise_platform(instance);
	bool graphics_ready = initialise_graphics();
	bool ui_ready = initialise_ui();

	if (platform_ready && graphics_ready)
	{
		int bp_shader = load_shader_program("vshader.glsl", "fshader.glsl");
		int terrain_shader = load_shader_program("vshader.glsl", "terrain_fshader.glsl");
		int terrain_lighting_shader = load_shader_program("vshader.glsl", "fshader.glsl");
		int heightmap_shader = load_shader_program("heightmap_vshader.glsl", "heightmap_fshader.glsl");
		int fire_shader = load_shader_program("fire_vshader.glsl", "fire_fshader.glsl");

		Camera main_view_camera = {};
		main_view_camera.set_position_and_target(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{ 0.0f, 0.0f, 1.0f });
		main_view_camera.movement_sensitivity = 0.1f;
		set_window_clear_colour(Vector3(0.52, 0.8, 0.92));

		activate_direction_light(0);
		set_direction_light_direction(0, Vector3(0.0f, -1.0f, -1.0f));
		set_direction_light_blinn_phong_properties(0, light_properties);

		Landscape_Data landscape = create_landscape(10.0f, 10.0f, 0.01f, 10);

		// Emit particles
		Emitter* fireEmitter = new Emitter({ -0.5,0.0,-0.5 }, 0.5, { 0.0,2.0,0.0 }, 10.0, 10.0, 10.0, 0.7, 1000, 1500, 0.01, 0.015);
		Emitter* fireEmitter2 = new Emitter({ 2.0,0.0,2.0 }, 0.5, { 0.0,2.0,0.0 }, 0.0, 0.0, 0.0, 0.7, 1000, 1500, 0.01, 0.015);
		ParticlePool pool;
		int totalNumOfParticles = 100000;
		initialisePool(pool, totalNumOfParticles);
		std::thread emit(releaseManyParticlesInASequenceForever, *fireEmitter, pool, 1000.0);
		std::thread emit2(releaseManyParticlesInASequenceForever, *fireEmitter2, pool, 1000.0);

		bool dragging = false;
		int fps = 60;
		bool render_wireframes = false;
		UI_Parameters ui_parameters = initialise_ui_parameter_pointers(&landscape, &main_view_camera, &fps, &render_wireframes);

		RigidBody* plane = new RigidBody({ 4.0,5.0,4.0 }, { -1.0,0.0,0.0 }, { 0.0,0.0,0.0 }, 10.0, 0.1);
		Vector3* planeRightVector = new Vector3{ plane->velocity.z, 0.0, -plane->velocity.x };
		float boostMultiplication = 0.0;

		timer t;
		//Main loop
		while (!should_window_close())
		{
			start_timer(&t);
			long int mspf = fps_to_mspf(fps);

			handle_input();
			handle_ui(ui_parameters);

			//if (was_key_pressed(KEY_UP)) main_view_camera.move_forward();
			//if (was_key_pressed(KEY_LEFT)) main_view_camera.move_left();
			//if (was_key_pressed(KEY_DOWN)) main_view_camera.move_backward();
			//if (was_key_pressed(KEY_RIGHT)) main_view_camera.move_right();
			if (was_key_pressed(KEY_W)) {
				Quaternion axis(*planeRightVector, 1.0);
				Matrix4x4 mat = quaternion_to_matrix(axis);
				Matrix3x3 mat3 = {};
				for (int i = 0; i <= 2; i++) {
					for (int j = 0; j <= 2; j++) {
						mat3[i][j] = mat[i][j];
					}
				}
				plane->velocity = mat3 * plane->velocity;
			}
			if (was_key_pressed(KEY_S)) {
				Quaternion axis(*planeRightVector, -1.0);
				Matrix4x4 mat = quaternion_to_matrix(axis);
				Matrix3x3 mat3 = {};
				for (int i = 0; i <= 2; i++) {
					for (int j = 0; j <= 2; j++) {
						mat3[i][j] = mat[i][j];
					}
				}
				plane->velocity = mat3 * plane->velocity;
			}
			if (was_key_pressed(KEY_A)) {
				Vector3 originalVelocity = plane->velocity;
				plane->velocity.x = (cos_deg(1) * originalVelocity.x) + (sin_deg(1) * originalVelocity.z);
				plane->velocity.z = (-sin_deg(1) * originalVelocity.x) + (cos_deg(1) * originalVelocity.z);
			}
			if (was_key_pressed(KEY_D)) {
				Vector3 originalVelocity = plane->velocity;
				plane->velocity.x = (cos_deg(-1) * originalVelocity.x) + (sin_deg(-1) * originalVelocity.z);
				plane->velocity.z = (-sin_deg(-1) * originalVelocity.x) + (cos_deg(-1) * originalVelocity.z);
			}
			if (was_key_pressed(KEY_RETURN)) {
				plane->velocity.y = 0.0;
			}
			if (was_key_pressed(KEY_SHIFT)) {
				if (boostMultiplication < 1.0) {
					boostMultiplication += 0.01;
				}
			}
			else {
				if (boostMultiplication > 0.0) {
					boostMultiplication -= 0.01;
				}
			}

			begin_render();
			if (render_wireframes) draw_as_wireframes();
			else draw_as_polygons();

			set_projection_matrix(perspective(50.0f, get_window_aspect_ratio(), 0.1f, 1000.0f));
			set_model_matrix(identity());
			use_shader(terrain_lighting_shader);

			buffer_camera_data_to_gpu(main_view_camera);

			float gravityRatio = (dot(plane->velocity, { 0.0,-1.0,0.0 })) / (length(plane->velocity));

			float downward = -1.0 * plane->mass * fabs(gravityRatio);
			Vector3 gravity = { 0.0,downward,0.0 };
			Vector3 thrust = 50.0 * boostMultiplication * normalise(plane->velocity);
			Vector3 forces[2] = { gravity, thrust };
			updateDisplacement(*plane, forces, 2, mspf / 1000.0);
			if (plane->displacement.y <= 0.0) {
				plane->velocity.y *= -0.5;
				plane->displacement.y = 0.01;
			}
			for (int i = 0; i <= 20; i++) {
				Vector2 treePos = landscape.forest.tree_distribution.landscape_positions[i];
				if (checkSphereCylinderCollision(*plane, treePos.x, treePos.y, 0.0, 25.0, 0.0001)) {
					plane->velocity.x = 0.0;
					plane->velocity.z = 0.0;
					break;
				}
			}

			OutputDebugStringf("%f\n", boostMultiplication);

			planeRightVector->x = plane->velocity.z;
			planeRightVector->z = -plane->velocity.x;

			main_view_camera.position = plane->displacement - (5.0 * normalise(plane->velocity));
			main_view_camera.forward = normalise(plane->velocity);
			main_view_camera.update_rightward_and_upward_vectors();

			if (main_view_camera.position.y <= 0.5) main_view_camera.position.y = 0.5;

			glLoadIdentity();
			glBegin(GL_QUADS);
			Vector3 point1 = plane->displacement + ((-0.1) * main_view_camera.rightward) + ((-0.1) * main_view_camera.upward);
			Vector3 point2 = plane->displacement + ((-0.1) * main_view_camera.rightward) + ((0.1) * main_view_camera.upward);
			Vector3 point3 = plane->displacement + ((0.1) * main_view_camera.rightward) + ((0.1) * main_view_camera.upward);
			Vector3 point4 = plane->displacement + ((0.1) * main_view_camera.rightward) + ((-0.1) * main_view_camera.upward);
			glVertex3f(point1.x, point1.y, point1.z);
			glVertex3f(point2.x, point2.y, point2.z);
			glVertex3f(point3.x, point3.y, point3.z);
			glVertex3f(point4.x, point4.y, point4.z);
			glEnd();

			//Draw landscape
			landscape.draw();
			render_ui();

			use_shader(fire_shader);

			// Check the particle pool
			int inactive = updatePool(pool, mspf);
			int loop = inactive;
			for (int i = 0; i <= loop; i++) {
				if (pool.nodes[i].nodeActive && pool.nodes[i].particle.life > 0) {
					float size = 0.0;
					// Have the particle decrease in size when it's coming to the end of its life
					if (pool.nodes[i].particle.life >= 100) {
						size = pool.nodes[i].particle.size;
					}
					else {
						size = pool.nodes[i].particle.size * (pool.nodes[i].particle.life / 100.0);
					}
					glBegin(GL_QUADS);
					Vector3 point1 = pool.nodes[i].particle.displacement + ((-size) * main_view_camera.rightward) + ((-size) * main_view_camera.upward);
					Vector3 point2 = pool.nodes[i].particle.displacement + ((-size) * main_view_camera.rightward) + ((size)*main_view_camera.upward);
					Vector3 point3 = pool.nodes[i].particle.displacement + ((size)*main_view_camera.rightward) + ((size)*main_view_camera.upward);
					Vector3 point4 = pool.nodes[i].particle.displacement + ((size)*main_view_camera.rightward) + ((-size) * main_view_camera.upward);
					glVertex3f(point1.x, point1.y, point1.z);
					glVertex3f(point2.x, point2.y, point2.z);
					glVertex3f(point3.x, point3.y, point3.z);
					glVertex3f(point4.x, point4.y, point4.z);
					glEnd();
				}
				else if (pool.nodes[i].nodeActive && pool.nodes[i].particle.life <= 0) {
					deleteParticleInPool(pool, i, inactive);
				}
			}

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
		emit.detach();
		emit2.detach();
		shutdown_platform();
		return 0;
	}
	else return 1;
}