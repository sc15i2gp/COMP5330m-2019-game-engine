#pragma once
#include <Windows.h>
#include <gl/GL.h>
#include "wglext.h"
#include "glext.h"

#define GL_REQUIRED_FUNCTIONS_LIST \
GL_FUNCTION(PFNGLUSEPROGRAMPROC, glUseProgram)

//Sets up opengl and returns whether initialisation was successful
bool initialise_opengl(HWND window);