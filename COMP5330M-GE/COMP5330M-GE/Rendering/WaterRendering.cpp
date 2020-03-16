#include "WaterRendering.h"
#include "../Graphics.h"

float vertexData[] = {

	-1.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f
};

WaterRendering::WaterRendering() : vertShader("./Rendering/waterShader.vert"), fragShader("./Rendering/waterShader.frag")
{
	/*Compiles, links and creates a GLuint ID for the shader program for water rendering*/
	program = load_shader_program(vertShader, fragShader);
}

void WaterRendering::renderWaterSurface()
{
	/*Generating a vao*/
	glGenVertexArrays(1, &waterVAO);

	/*Create a vertex buffer object*/
	glGenBuffers(1, &waterVBO);

	/*So, the first thing we bind is a VAO*/
	glBindVertexArray(waterVAO);

	/*And only then begin binding the corresponding VBOs which the VAO would use*/
	glBindBuffer(GL_ARRAY_BUFFER, waterVBO);

	/*Copy the data over*/
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	/*Be careful with this call, the first one is the layout(location = <someNumber> used inside the vertex shader*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	/*Safely unbind the data used*/
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}


GLuint WaterRendering::getProgramID() const
{
	return program;
}

GLuint WaterRendering::getVAO() const
{
	return waterVAO;
}