#include <stdio.h>
#include <Windows.h>
#include "OpenGL.h"

//TODO:
//	- printf_debug to OutputDebugString but with formatted strings

bool running = true;

char* read_file(const char* path)
{
	FILE* f = fopen(path, "rb");
	if (!f) return NULL;

	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* contents = (char*)malloc(size+1);
	if (contents)
	{
		fread(contents, 1, size, f);
		contents[size] = 0;
	}

	fclose(f);
	return contents;
}

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

		//Load shader
		char* v_shader_src = read_file("vshader.glsl");
		char* f_shader_src = read_file("fshader.glsl");
		char shader_src[2048] = {};
		sprintf_s(shader_src, "Vertex Shader:\n%s\n", v_shader_src);
		OutputDebugString(shader_src);
		sprintf_s(shader_src, "Fragment Shader:\n%s\n", f_shader_src);
		OutputDebugString(shader_src);

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