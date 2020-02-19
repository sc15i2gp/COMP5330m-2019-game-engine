#include "Platform.h"
#include "Graphics.h"
#include "Maths.h"
#include<iostream>

//TODO: Regulate "normal" graphical pipeline
//	- Blinn-Phong lighting
//	- Provide GUI for world matrix and camera control

//TODO: 

// Add a quad mesh with enough vertices
// Add Sine Wave to imitiate waves
// Figure out how the other type displacement happens
// Add that displacement to the full thing
// Add texture-mapping
// Figure out howto add two textures simultaniously 
// Make instances of this water quad

//DEBUGGING FOR SHADERS

//Triangle not rendering, fix the issues
// Translate code as how Isaac has done it (?)

void print_mat(Matrix4x4 m)
{
	for (int i = 0; i < 4; ++i)
	{
		OutputDebugStringf("%f %f %f %f\n", m[0][i], m[1][i], m[2][i], m[3][i]);
	}
}

float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f,  0.5f, 0.0f
};

//Windows entry point
int WINAPI WinMain(HINSTANCE instance, HINSTANCE previous_instance, LPSTR cmd_line, int nCmdShow)
{
	bool platform_ready = initialise_platform(instance);
	bool graphics_ready = initialise_graphics();

	// build and compile our shader program
	// ------------------------------------
	// vertex shader
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexShaderSource = read_file("testVertexShader.glsl");
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	// check for shader compile errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		OutputDebugStringA(infoLog);
	}
	// fragment shader
	int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentShaderSource = read_file("testFragShader.glsl");
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	// check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		OutputDebugStringA(infoLog);
	}
	// link shaders
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	// check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		OutputDebugStringA(infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	float vertices[] = {
		0.9f,  0.9f, 0.0f,  // top right
		0.9f, -0.9f, 0.0f,  // bottom right
		-0.9f, -0.9f, 0.0f,  // bottom left
		-0.9f,  0.9f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);



	glBindVertexArray(0);

	
	/*Draws the surface in wire-frame mode*/
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (platform_ready && graphics_ready)
	{

		//Main loop
		while (!should_window_close()) 
		{
			handle_input();

			glClearColor(131.0f / 255.0f, 122.0f / 255.0f, 122.0f / 255.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glUseProgram(shaderProgram);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


			swap_window_buffers();
		}

		/*Release resources before shut down*/
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

		//ReleaseDC(window, window_device_context);
		shutdown_platform();
		return 0;
	}
	else return 1;
}