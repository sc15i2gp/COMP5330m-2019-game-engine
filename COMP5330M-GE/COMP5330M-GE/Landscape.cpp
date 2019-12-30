#include "Landscape.h"

//Buffer a regularly spaced grid of vertices, centered around (0, 0, z)
Drawable buffer_terrain_mesh(float terrain_width, float terrain_length, float cell_length)
{
	OutputDebugStringf("Terrain Width: %f\nTerrain Length: %f\n", terrain_width, terrain_length);


	Perlin_Noise_Function perlin_noise = generate_noise_function(terrain_width, terrain_length, cell_length, cell_length);

	int vertex_width = (int)(ceil(terrain_width / cell_length)) + 1;
	int vertex_length = (int)(ceil(terrain_length / cell_length)) + 1;

	int number_of_vertices = vertex_width * vertex_length;
	OutputDebugStringf("Number of vertices in terrain = %d\n", number_of_vertices);
	OutputDebugStringf("Vertex Dimensions: %d %d\n", vertex_width, vertex_length);
	float actual_terrain_width = (float)(vertex_width - 1) * cell_length;
	float actual_terrain_length = (float)(vertex_length - 1) * cell_length;
	Vector3 O(-actual_terrain_width / 2.0f, 0.0f, actual_terrain_length / 2.0f);

	Mesh_vertex* vertices = (Mesh_vertex*)alloc_mem(number_of_vertices * sizeof(Mesh_vertex));
	for (int i = 0; i < vertex_length; ++i)
	{
		for (int j = 0; j < vertex_width; ++j)
		{
			float v_x = (float)j * cell_length;
			float v_z = (float)i * cell_length;
			vertices[i*vertex_width + j].position = O + Vector3(v_x, 0.0f, -v_z);
			vertices[i*vertex_width + j].position.y = perlin_noise(v_x, v_z);
			vertices[i*vertex_width + j].normal = Vector3(0.f, 1.0f, 0.0f);
		}
	}


	OutputDebugStringf("Vertex offset = %d\n", (vertex_length - 1)*vertex_width + vertex_width - 1);
	perlin_noise(40, 30);
	//Number of indices = indices per triangle * triangles per rectangle * number of rectangles in grid
	int number_of_rectangles = (vertex_width - 1)*(vertex_length - 1);
	int number_of_triangles = 2 * number_of_rectangles;
	int number_of_indices = 3 * number_of_triangles;
	GLuint* indices = (GLuint*)alloc_mem(3 * number_of_triangles * sizeof(GLuint));

	for (int i = 0; i < number_of_rectangles; ++i)
	{
		GLuint r_0 = (i / (vertex_width - 1)) * vertex_width + (i % (vertex_width - 1));
		GLuint r_1 = r_0 + 1;
		GLuint r_2 = r_1 + vertex_width;
		GLuint r_3 = r_2 - 1;

		GLuint rect_indices[] = { r_0, r_1, r_2, r_2, r_3, r_0 };
		for (int j = 0; j < 6; ++j)
		{
			indices[6 * i + j] = rect_indices[j];
		}
	}

	OutputDebugStringf("Number of indices = %d\n", 3 * number_of_triangles);
	OutputDebugStringf("Index offset = %d\n", 6 * (number_of_rectangles - 1) + 5);

	//for(int i = 0; i < number_of_vertices; ++i)
	OutputDebugStringf("V: %f\n", perlin_noise(40, 40));
	Drawable terrain = buffer_mesh(vertices, number_of_vertices, indices, number_of_indices);

	destroy_noise_function(perlin_noise);

	dealloc_mem(indices);
	dealloc_mem(vertices);

	return terrain;
}
