#include "OpenGL.h"

//To render, a window's pixel format needs to be set. However, we can't just give a format, we have to use one given
//to us by the OS. Therefore, a desired format is specified and the OS suggests one back, the one suggested we use.
bool set_window_pixel_format(HDC window_device_context)
{
	PIXELFORMATDESCRIPTOR desired_pixel_format = {}; //The pixel format we want
	PIXELFORMATDESCRIPTOR suggested_pixel_format = {}; //The pixel format the OS suggests
	desired_pixel_format.nSize = sizeof(desired_pixel_format);
	desired_pixel_format.nVersion = 1;
	desired_pixel_format.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	desired_pixel_format.cColorBits = 24; //3 bytes for RGB
	desired_pixel_format.cAlphaBits = 8; //1 byte for A
	desired_pixel_format.iLayerType = PFD_MAIN_PLANE;

	//Find index for suggested pixel format
	int suggested_pixel_format_index = ChoosePixelFormat(window_device_context, &desired_pixel_format);
	//Retrieve OS' suggested pixel format
	DescribePixelFormat(window_device_context, suggested_pixel_format_index, sizeof(suggested_pixel_format), &suggested_pixel_format);
	//Set the pixel format to that
	SetPixelFormat(window_device_context, suggested_pixel_format_index, &suggested_pixel_format);

	//TODO: Error handling for when the suggested pixel format is not acceptable

	return true;
}

bool create_gl_context(HDC window_device_context)
{
	HGLRC gl_context = wglCreateContext(window_device_context);
	return wglMakeCurrent(window_device_context, gl_context);
}

bool load_opengl_function_ptrs()
{
	return true;
}

bool initialise_opengl(HWND window)
{
	HDC window_device_context = GetDC(window);

	set_window_pixel_format(window_device_context);
	create_gl_context(window_device_context);
	load_opengl_function_ptrs();

	return true;
}