#pragma once
#include <stdio.h>
#include <Windows.h>
#include <gl/GL.h>
#include "wglext.h"
#include "glext.h"

//List of gl functions that we need
//WARNING: Please don't touch without talking to Isaac first
#define GL_REQUIRED_FUNCTIONS_LIST \
GL_FUNCTION(PFNGLUSEPROGRAMPROC, glUseProgram) \
GL_FUNCTION(PFNGLSHADERSOURCEPROC, glShaderSource) \
GL_FUNCTION(PFNGLCREATESHADERPROC, glCreateShader) \
GL_FUNCTION(PFNGLCOMPILESHADERPROC, glCompileShader) \
GL_FUNCTION(PFNGLGETSHADERIVPROC, glGetShaderiv) \
GL_FUNCTION(PFNGLCREATEPROGRAMPROC, glCreateProgram) \
GL_FUNCTION(PFNGLLINKPROGRAMPROC, glLinkProgram) \
GL_FUNCTION(PFNGLGETPROGRAMIVPROC, glGetProgramiv) \
GL_FUNCTION(PFNGLATTACHSHADERPROC, glAttachShader) \
GL_FUNCTION(PFNGLDELETESHADERPROC, glDeleteShader) \
GL_FUNCTION(PFNGLGENBUFFERSPROC, glGenBuffers) \
GL_FUNCTION(PFNGLDELETEBUFFERSPROC, glDeleteBuffers) \
GL_FUNCTION(PFNGLBINDBUFFERPROC, glBindBuffer) \
GL_FUNCTION(PFNGLBUFFERDATAPROC, glBufferData) \
GL_FUNCTION(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays) \
GL_FUNCTION(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays) \
GL_FUNCTION(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray) \
GL_FUNCTION(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer) \
GL_FUNCTION(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray) \
GL_FUNCTION(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog)

#define GL_FUNCTION(FUNCTION_TYPE, FUNCTION_NAME) extern FUNCTION_TYPE FUNCTION_NAME;
GL_REQUIRED_FUNCTIONS_LIST
#undef GL_FUNCTION

GLuint create_shader_program(char* vertex_shader_src, char* fragment_shader_src);

//Sets up opengl and returns whether initialisation was successful
bool initialise_opengl(HWND window);