#include "WaterRendering.h"
#include "Framework/Graphics.h"

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

	//generateData(Vector2(0.0f, 0.0f), 10, 10, 3);

	std::vector<Vertex> grid = generateCells(Vector3(+6.0f, 0.0f, +6.0f), 2.0f, 2.0f, 32, 32);
	const uint32_t gridSize = grid.size() * sizeof(Vector3) + grid.size() * sizeof(RGBAColour);
	gridVertices = grid.size();

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
	glDrawArrays(GL_TRIANGLES, 0, gridVertices);
}

std::vector<Vertex> WaterRendering::generateCells(const Vector3 & startPoint, const float quadWidth, const float quadHeight, const unsigned int cellsAlongX, const unsigned int cellsAlongY)
{
	std::vector<Vertex> data;

	//RGBAColour red(1.0f, 0.0f, 0.0f, 1.0f);
	//RGBAColour green(0.0f, 1.0f, 0.0f, 1.0f);
	//RGBAColour blue(0.0f, 0.0f, 1.0f, 1.0f);
	//RGBAColour yellow(0.0f, 1.0f, 1.0f, 1.0f);


	/****************************************************************************/
	/*

				pos1  ------------- pos0
					  | \         |
					  |	   \      |
					  |       \   |
				pos2  ------------- pos3



	*/
	/***************************************************************************/

	for (uint32_t cellX = 0; cellX < cellsAlongX; cellX++)
	{
		for (uint32_t cellY = 0; cellY < cellsAlongY; cellY++)
		{
			const uint32_t cellOffsetAlongX = cellX * quadWidth;
			const uint32_t cellsOffsetAlongY = cellY * quadHeight;

			/*The positions of each vertex in the quad*/
			Vector3 pos0(startPoint.x + cellOffsetAlongX, startPoint.y, startPoint.z + cellsOffsetAlongY);
			Vector3 pos1(startPoint.x + cellOffsetAlongX + quadWidth, startPoint.y, startPoint.z + cellsOffsetAlongY);
			Vector3 pos2(startPoint.x + cellOffsetAlongX + quadWidth, startPoint.y, startPoint.z + cellsOffsetAlongY + quadHeight);
			Vector3 pos3(startPoint.x + cellOffsetAlongX, startPoint.y, startPoint.z  + cellsOffsetAlongY + quadHeight);

			RGBAColour colour(1.0f, 1.0f, 1.0f, 1.0f);

			if (cellX == 0 && cellY == 0)
			{
				colour.red = 1.0f;
				colour.green = 0.0f;
				colour.blue = 0.0f;
			}
			if (cellX == 0 && cellY == 1)
			{
				colour.red = 0.0f;
				colour.green = 1.0f;
				colour.blue = 0.0f;
			}
			if (cellX == 1 && cellY == 0)
			{
				colour.red = 1.0f;
				colour.green = 0.0f;
				colour.blue = 1.0f;
			}
			if (cellX == 1 && cellY == 1)
			{
				colour.red = 1.0f;
				colour.green = 1.0f;
				colour.blue = 0.0f;
			}

			/*Each quad gets made up from 2 individual triangles*/
			Vertex v(pos0, colour);
			Vertex v1(pos1, colour);
			Vertex v2(pos3, colour);

			Vertex v3(pos1, colour);
			Vertex v4(pos2, colour);
			Vertex v5(pos3, colour);

			/*Add the data to the array*/
			data.push_back(v);
			data.push_back(v1);
			data.push_back(v2);

			data.push_back(v3);
			data.push_back(v4);
			data.push_back(v5);
		}
	}

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