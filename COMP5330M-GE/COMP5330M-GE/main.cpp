#include "Platform.h"
#include "OpenGL.h"
#include "Maths.h"
#include "RigidBody.h"

void set_uniform_mat4(GLuint shader, const char* uniform_name, GLfloat* matrix)
{
	GLint matrix_location = glGetUniformLocation(shader, uniform_name);
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, matrix);
}

bool running = true;

//Function is called for every event passed to the process by Windows
LRESULT CALLBACK window_event_handler(HWND window, UINT message, WPARAM wparam, LPARAM lparam)
{
	LRESULT result = 0;
	switch (message)
	{
	case WM_CLOSE: //Case: Window 'X' has been pressed
		running = false; //Set the program to stop running
		break;
	default: //Anything else
		result = DefWindowProc(window, message, wparam, lparam); //Call the default window handling routine for the given message
		break;
	}
	return result;
}

//Creates a window for the given instance
HWND open_program_window(HINSTANCE instance)
{
	//Create window class
	WNDCLASS window_class = {};
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpfnWndProc = window_event_handler;
	window_class.lpszClassName = "GameEngineClass";

	//Attempt to register the window class
	if (RegisterClass(&window_class))
	{//If the class registation was successful
		HWND window = CreateWindow(window_class.lpszClassName, "Game Engine Test", 
			WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, 0, 0, instance, 0);
		return window;
	}
	else return NULL;
}

void print_mat(Matrix4x4 m)
{
	OutputDebugString("Matrix:\n");
	for (int i = 0; i < 4; ++i)
	{
		Vector4 r = m.row(i);
		OutputDebugStringf("%f %f %f %f\n", r.x, r.y, r.z, r.w);
	}
}

//Windows entry point
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR cmd_line, int nCmdShow)
{
	HWND window = open_program_window(instance);
	if (window)
	{//If the window was successfully created
		//Initialise opengl
		if (!initialise_opengl(window))
		{
			//TODO: Error handling
			OutputDebugString("Error: Unable to initialise OpenGL!\n");
			return 1;
		}

		Vector4 v = { 0.0f, 0.0f, 0.0f, 1.0f };
		Vector4 w = { 1.0f, 1.0f, 1.0f, 1.0f };
		v += w;
		OutputDebugStringf("Vector: %f %f %f %f\n", v[0], v[1], v[2], v[3]);

		//Load shader
		char* v_shader_src = read_file("vshader.glsl");
		char* f_shader_src = read_file("fshader.glsl");

		OutputDebugStringf("Vertex Shader : \n%s\n", v_shader_src);
		OutputDebugStringf("Fragment Shader:\n%s\n", f_shader_src);

		GLuint shader = create_shader_program(v_shader_src, f_shader_src);
		free(f_shader_src);
		free(v_shader_src);
		
		//Load triangle data to GPU
		GLfloat vertices[] =
		{
			0.0f, 1.0f, //Top
			-1.0f, -1.0f, //Left
			1.0f, -1.0f //Right
		};

		GLuint VAO, VBO;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);
		Matrix4x4 model = identity();
		translate(model, Vector3(0.0f, 0.2f, 0.0f));

		Matrix4x4 view = look_at(Vector3(0.0f, 0.0f, 1.0f), Vector3(0.0f, 0.0f, 0.0f));
		//Matrix4x4 view = identity();

		OutputDebugStringf("Aspect ratio: %f\n", get_aspect_ratio(window));
		Matrix4x4 projection = perspective(90.0f, get_aspect_ratio(window), 0.1f, 10.0f);
		print_mat(projection);
		print_mat(view);
		glUseProgram(shader);
		set_uniform_mat4(shader, "model", &model[0][0]);
		set_uniform_mat4(shader, "view", &view[0][0]);
		set_uniform_mat4(shader, "projection", &projection[0][0]);

		// Physics Testing goes here
		//RigidBody point({ 0.0,20.0,0.0 }, { 0.0,0.0,0.0 }, { 0.0,0.0,0.0 }, 10.0);
		//Vector3 gravity(0.0, -9.81 * point.mass, 0.0);
		//Vector3* forces = { &gravity };
		//for (int i = 0; i < 300; i++) {
		//	updateDisplacement(point, forces, 1, 0.01667);
		//	OutputDebugStringf("%f %f %f\n", point.displacement.x, point.displacement.y, point.displacement.z);
		//	i++;
		//}

		//Main loop
		HDC window_device_context = GetDC(window); //Used to swap buffers
		while (running)
		{
			//Handle input events
			MSG message;
			while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
			{//Process each input event message given since the last frame
				//Push messages to thread's input event queue
				TranslateMessage(&message);
				//Send the message to the window event handler function
				DispatchMessage(&message);

				//Draw triangle
				glClearColor(0.98f, 0.85f, 0.86f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glUseProgram(shader);
				glBindVertexArray(VAO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glDrawArrays(GL_TRIANGLES, 0, 3);
				glBindVertexArray(0);

				//Swap window buffers
				SwapBuffers(window_device_context);
			}
		}
		ReleaseDC(window, window_device_context);
	}
	else
	{//If the window wasn't created
		//TODO: Better error handling here (if possible)
		OutputDebugString("Error: Window could not be created!\n");
		return 1;
	}
	return 0;
}