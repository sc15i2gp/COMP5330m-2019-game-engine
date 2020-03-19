#include "WaterRendering.h"
#include "../Graphics.h"

float vertices[] = {
	2.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	2.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

	0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	0.0f, 2.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	2.0f, 2.0f, 0.0f, 0.0f, 1.0f, 0.0f

};

WaterRendering::WaterRendering() : vertShader("./Rendering/waterShader.vert"), fragShader("./Rendering/waterShader.frag")
{
	/*Compiles, links and creates a GLuint ID for the shader program for water rendering*/
	program = load_shader_program(vertShader, fragShader);
}

void WaterRendering::renderWaterSurface()
{

	float vertices[] = {

		//pos                  //col
		0.5f, 0.5f, 0.0f,	  1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.0f,	  0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.0f,	  0.0f, 0.0f, 1.0f, 1.0f,
	};

	//generateData(Vector2(0.0f, 0.0f), 10, 10, 3);

	std::vector<Vertex> grid = generateCells(Vector3(0.0f, 0.0f, 0.0f), 3.0f, 3.0f, 5, 5);
	const uint32_t gridSize = grid.size() * sizeof(Vector3) + grid.size() * sizeof(RGBAColour);
	/*Generating a vao*/
	glGenVertexArrays(1, &waterVAO);

	/*Create a vertex buffer object*/
	glGenBuffers(1, &waterVBO);

	/*Generate IBO*/
	glGenBuffers(1, &waterIBO);

	/*So, the first thing we bind is a VAO*/
	glBindVertexArray(waterVAO);

	/*And only then begin binding the corresponding VBOs which the VAO would use*/
	glBindBuffer(GL_ARRAY_BUFFER, waterVBO);

	/*Copy the data over*/
	glBufferData(GL_ARRAY_BUFFER, gridSize, &grid[0], GL_STATIC_DRAW);


	/*Be careful with this call, the first one is the layout(location = <someNumber> used inside the vertex shader*/
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex) , nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	/*Safely unbind the data used*/
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void WaterRendering::drawWater()
{
	use_shader(program);
	glBindVertexArray(waterVAO);
	glDisable(GL_CULL_FACE);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

std::vector<Vertex> WaterRendering::generateCells(const Vector3 & startPoint, const float quadWidth, const float quadHeight, const unsigned int cellsAlongX, const unsigned int cellsAlongY)
{
	std::vector<Vertex> data;

	RGBAColour red(1.0f, 0.0f, 0.0f, 1.0f);
	RGBAColour green(0.0f, 1.0f, 0.0f, 1.0f);
	RGBAColour blue(0.0f, 0.0f, 1.0f, 1.0f);
	RGBAColour yellow(0.0f, 1.0f, 1.0f, 1.0f);

	Vector3 pos0(0.5f, 0.5f, 0.0f);
	Vector3 pos1(0.5f, -0.5f, 0.0f);
	Vector3 pos2(-0.5f, -0.5f, 0.0f);
	Vector3 pos3(-0.5f, 0.5f, 0.0f);

	Vertex v(pos0, red);
	Vertex v1(pos1, green);
	Vertex v2(pos2, blue);

	Vertex v3(pos2, blue);
	Vertex v4(pos3, yellow);
	Vertex v5(pos0, red);


	data.push_back(v);
	data.push_back(v1);
	data.push_back(v2);

	data.push_back(v3);
	data.push_back(v4);
	data.push_back(v5);



	return data;
}

GLuint WaterRendering::getProgramID() const
{
	return program;
}

GLuint WaterRendering::getVAO() const
{
	return waterVAO;
}