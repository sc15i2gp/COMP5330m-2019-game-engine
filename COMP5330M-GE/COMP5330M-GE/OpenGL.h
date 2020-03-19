#pragma once
#include "Platform.h"

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
GL_FUNCTION(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog)\
GL_FUNCTION(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation) \
GL_FUNCTION(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv) \
GL_FUNCTION(PFNGLBLENDEQUATIONPROC, glBlendEquation) \
GL_FUNCTION(PFNGLUNIFORM1IPROC, glUniform1i) \
GL_FUNCTION(PFNGLBINDSAMPLERPROC, glBindSampler) \
GL_FUNCTION(PFNGLACTIVETEXTUREPROC, glActiveTexture) \
GL_FUNCTION(PFNGLDRAWELEMENTSBASEVERTEXPROC, glDrawElementsBaseVertex) \
GL_FUNCTION(PFNGLBLENDEQUATIONSEPARATEPROC, glBlendEquationSeparate) \
GL_FUNCTION(PFNGLBLENDFUNCSEPARATEPROC, glBlendFuncSeparate) \
GL_FUNCTION(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog) \
GL_FUNCTION(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation) \
GL_FUNCTION(PFNGLDETACHSHADERPROC, glDetachShader) \
GL_FUNCTION(PFNGLDELETEPROGRAMPROC, glDeleteProgram) \
GL_FUNCTION(PFNGLUNIFORM3FVPROC, glUniform3fv) \
GL_FUNCTION(PFNGLUNIFORM1FPROC, glUniform1fv) \
GL_FUNCTION(PFNGLMAPBUFFERRANGEPROC, glMapBufferRange) \
GL_FUNCTION(PFNGLUNMAPBUFFERPROC, glUnmapBuffer) \
GL_FUNCTION(PFNGLBINDBUFFERBASEPROC, glBindBufferBase) \
GL_FUNCTION(PFNGLGENERATEMIPMAPPROC, glGenerateMipmap)

#define GL_FUNCTION(FUNCTION_TYPE, FUNCTION_NAME) extern FUNCTION_TYPE FUNCTION_NAME;
GL_REQUIRED_FUNCTIONS_LIST
#undef GL_FUNCTION

//Sets up opengl and returns whether initialisation was successful
bool initialise_opengl(HWND window);

void opengl_check_for_errors();