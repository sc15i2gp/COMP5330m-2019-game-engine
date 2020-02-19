#include "Platform.h"
#include "Graphics.h"
#include "Maths.h"

#include<iostream>
#include "Landscape.h"
#include "l_system.h"
#include "turtle.h"
#include "Camera.h"
#include "UI.h"

//DOING:

//TODO: 

// Add a quad mesh with enough vertices
// Add Sine Wave to imitiate waves
// Figure out how the other type displacement happens
// Add that displacement to the full thing
// Add texture-mapping
// Figure out howto add two textures simultaniously 
// Make instances of this water quad

//DEBUGGING FOR SHADERS

//Triangle not rendering, fix the issues
// Translate code as how Isaac has done it (?)


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


float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f,  0.5f, 0.0f
};


//long int fps_to_mspf(int fps)
//{
//	float spf = 1.0f / (float)fps;
//	float mspf = 1000.0f*spf;
//	return (long int)mspf;
//}
//
//struct timer
//{
//	LARGE_INTEGER clock_frequency;
//	LARGE_INTEGER start_time;
//	LARGE_INTEGER stop_time;
//};
//
//void start_timer(timer* t)
//{
//	QueryPerformanceFrequency(&t->clock_frequency);
//	QueryPerformanceCounter(&t->start_time);
//}
//
//void stop_timer(timer* t)
//{
//	QueryPerformanceCounter(&t->stop_time);
//}
//
//long int elapsed_time(timer* t)
//{
//	LARGE_INTEGER elapsed;
//	elapsed.QuadPart = t->stop_time.QuadPart - t->start_time.QuadPart;
//	elapsed.QuadPart *= 1000;
//	elapsed.QuadPart /= t->clock_frequency.QuadPart;
//
//	return elapsed.QuadPart;
//}

//Windows entry point
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR cmd_line, int nCmdShow)
{
	bool platform_ready = initialise_platform(instance);
	bool graphics_ready = initialise_graphics();

	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSource = read_file("testVertexShader.glsl");
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		OutputDebugStringA(infoLog);
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSource = read_file("testFragShader.glsl");
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		OutputDebugStringA(infoLog);
	}
	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		OutputDebugStringA(infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	float vertices[] = {
		0.9f,  0.9f, 0.0f,  // top right
		0.9f, -0.9f, 0.0f,  // bottom right
		-0.9f, -0.9f, 0.0f,  // bottom left
		-0.9f,  0.9f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);



	glBindVertexArray(0);


	/*Draws the surface in wire-frame mode*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (platform_ready && graphics_ready)
	{
		Camera main_view_camera = {};
		main_view_camera.set_position_and_target(Vector3{ 0.0f, 0.0f, 0.0f }, Vector3{0.0f, 0.0f, 1.0f});
		main_view_camera.movement_sensitivity = 0.1f;

		activate_direction_light(0);
		set_direction_light_direction(0, Vector3(0.0f, -1.0f, -1.0f));
		set_direction_light_blinn_phong_properties(0, light_properties);

		bool dragging = false;
		int fps = 60;
		bool render_wireframes = false;


		while (!should_window_close()) 
		{

			handle_input();

			//if (was_mouse_button_pressed(BUTTON_LEFT) && was_mouse_moved() && !dragging)
			//{
			//	dragging = true;
			//	main_view_camera.rotation_start(get_initial_mouse_position());
			//}
			//if (was_mouse_moved() && dragging)
			//{
			//	//main_view_camera.rotation_start(get_initial_mouse_position());
			//	main_view_camera.rotate_by_arcball(get_final_mouse_position());
			//}
			//if (!was_mouse_button_pressed(BUTTON_LEFT) && dragging)
			//{
			//	main_view_camera.rotation_end();
			//	dragging = false;
			//}

			//if (was_key_pressed(KEY_W)) main_view_camera.move_forward();
			//if (was_key_pressed(KEY_A)) main_view_camera.move_left();
			//if (was_key_pressed(KEY_S)) main_view_camera.move_backward();
			//if (was_key_pressed(KEY_D)) main_view_camera.move_right();
			//if (was_key_pressed(KEY_Q)) main_view_camera.move_up();
			//if (was_key_pressed(KEY_E)) main_view_camera.move_down();

			glClearColor(131.0f / 255.0f, 122.0f / 255.0f, 122.0f / 255.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


			/*begin_render();*/

		/*	set_projection_matrix(perspective(50.0f, get_window_aspect_ratio(), 0.1f, 1000.0f));
			set_model_matrix(identity());
*/
			//buffer_camera_data_to_gpu(main_view_camera);

			swap_window_buffers();
		}

		/*Release resources before shut down*/
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

		//release_drawable();
		//ReleaseDC(window, window_device_context);
		shutdown_platform();
		return 0;
	}
	else return 1;
}