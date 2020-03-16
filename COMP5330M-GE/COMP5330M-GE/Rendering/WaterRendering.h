#pragma once

#include "../OpenGL.h"; // OpenGL functions
#include <string>

class WaterRendering
{
private:

	const char* vertShader;
	const char* fragShader;

	GLuint program;

	GLuint waterVBO;
	GLuint waterVAO;

public:

	WaterRendering();

	void renderWaterSurface();

	GLuint getProgramID() const;
	GLuint getVAO() const;

};