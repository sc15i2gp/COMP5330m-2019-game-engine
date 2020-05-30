#pragma once

#include "../Framework/OpenGL.h"; // OpenGL functions
#include <string>
#include <vector>

#include "../Tools/Vertex.h"
#include "Tools/Texture.h"

class WaterRendering
{
private:

	const char* vertShader;
	const char* fragShader;

	std::vector<Vertex> vertexData;

	GLuint program;

	GLuint waterVBO; // Buffer object
	GLuint waterVAO; // Vertex Buffer Object
	GLuint waterIBO; // Index buffer

	uint32_t gridVertices;

	Texture m_waterTexture;

public:

	WaterRendering();

	void renderWaterSurface();

	void drawWater();

	//std::vector<Vertex> generateData(const Vector2 & startPoint, const uint32_t width, const uint32_t height, const uint32_t offset);
	std::vector<Vertex> generateCells(const Vector3& startPoint, const float quadWidth, const float quadHeight, const unsigned int cellsAlongX, const unsigned int cellsAlongY);

	GLuint getProgramID() const;
	GLuint getVAO() const;
};