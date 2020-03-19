#include "OpenGL.h"

#define GL_FUNCTION(FUNCTION_TYPE, FUNCTION_NAME) FUNCTION_TYPE FUNCTION_NAME;
GL_REQUIRED_FUNCTIONS_LIST
#undef GL_FUNCTION

/************************************/

/*			Initialisation			*/

/************************************/

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
	//const int versionMajor = 4;
	//const int versionMinor = 0;

	//// In order to load the function, we have to first create a context, which we won't use for the main application
	//PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;
	//wglCreateContextAttribsARB = reinterpret_cast<PFNWGLCREATECONTEXTATTRIBSARBPROC>(wglGetProcAddress("wglCreateContextAttribsARB"));

	//if (wglCreateContextAttribsARB == nullptr)
	//{
	//	MessageBox(0, "Failed to load wglContextAttribsARB", "Window::create", MB_ICONERROR);
	//	return false;
	//}

	//int  contextAttributes[] = {
	//WGL_CONTEXT_MAJOR_VERSION_ARB, versionMajor,
	//WGL_CONTEXT_MINOR_VERSION_ARB, versionMinor,
	//WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
	//0
	//};

	HGLRC gl_context = wglCreateContext(window_device_context);
	return wglMakeCurrent(window_device_context, gl_context);
}


bool load_opengl_function_ptrs()
{
	bool functions_successfully_loaded = true;

	//Defines a block of code to be expanded for each of the required gl functions
	//For each gl function, it gets the function's address (wglGetProcAddress)
	//If the function's address wasn't retrieved, output error message and return false
	#define GL_FUNCTION(FUNCTION_TYPE, FUNCTION_NAME) \
	FUNCTION_NAME = (FUNCTION_TYPE)wglGetProcAddress(#FUNCTION_NAME); \
	if(!FUNCTION_NAME) \
	{ \
		OutputDebugString("Failed to get address of function: "#FUNCTION_NAME"\n"); \
		functions_successfully_loaded = false; \
	} \
	else \
	{\
		OutputDebugString("Acquired address of function: "#FUNCTION_NAME"\n"); \
	}
	GL_REQUIRED_FUNCTIONS_LIST
	#undef GL_FUNCTION

	return functions_successfully_loaded;
}

bool initialise_opengl(HWND window)
{
	HDC window_device_context = GetDC(window);

	bool pixel_format_set = set_window_pixel_format(window_device_context);
	bool gl_context_created = create_gl_context(window_device_context);
	bool gl_functions_loaded = load_opengl_function_ptrs();

	return pixel_format_set && gl_context_created && gl_functions_loaded;
}

void opengl_check_for_errors()
{
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR)
	{
		OutputDebugStringf("OpenGL error: %x\n", error);
	}
}



