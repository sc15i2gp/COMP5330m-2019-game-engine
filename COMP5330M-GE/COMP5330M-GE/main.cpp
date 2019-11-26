#include <stdio.h>
#include <Windows.h>
#include "OpenGL.h"
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
		
		//Load triangle data to GPU

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