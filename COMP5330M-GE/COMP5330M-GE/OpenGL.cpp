#include "OpenGL.h"

#define GL_FUNCTION(FUNCTION_TYPE, FUNCTION_NAME) FUNCTION_TYPE FUNCTION_NAME;
GL_REQUIRED_FUNCTIONS_LIST
#undef GL_FUNCTION

/****************************/

/*			Utility			*/

/****************************/

GLuint compile_shader_src(char* shader_src, GLenum shader_type)
{
	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &shader_src, NULL);
	glCompileShader(shader);

	char info[512] = {};
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, info);
		OutputDebugStringf("Error: Shader compilation failed: \n%s", info);
		return 0;
	}
	return shader;
}

GLuint create_shader_program(char* v_shader_src, char* f_shader_src)
{
	GLuint v_shader = compile_shader_src(v_shader_src, GL_VERTEX_SHADER);
	GLuint f_shader = compile_shader_src(f_shader_src, GL_FRAGMENT_SHADER);

	GLuint shader = glCreateProgram();
	glAttachShader(shader, v_shader);
	glAttachShader(shader, f_shader);
	glLinkProgram(shader);

	GLint success;
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success)
	{//If the shader program wasn't linked
		OutputDebugString("Failed to link shader program\n");
		return 0;
	}
	glDeleteShader(v_shader);
	glDeleteShader(f_shader);
	return shader;
}


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