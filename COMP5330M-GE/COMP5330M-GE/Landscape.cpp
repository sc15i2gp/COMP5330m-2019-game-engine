#include "Landscape.h"

float normalise_range(float f)
{
	return (f + 1.0f) / 2.0f;
}

//Buffer a regularly spaced grid of vertices, centered around (0, 0, z)
Terrain create_terrain(float terrain_width, float terrain_length, float terrain_cell_length, float frequency)
{
	Terrain terrain = {};
	terrain.width = terrain_width;
	terrain.length = terrain_length;
	OutputDebugStringf("Terrain Width: %f\nTerrain Length: %f\n", terrain_width, terrain_length);

	terrain.perlin_noise = generate_noise_function();

	int vertex_width = (int)(ceil(terrain_width / terrain_cell_length)) + 1;
	int vertex_length = (int)(ceil(terrain_length / terrain_cell_length)) + 1;

	terrain.mesh.number_of_vertices = vertex_width * vertex_length;
	OutputDebugStringf("Number of vertices in terrain = %d\n", terrain.mesh.number_of_vertices);
	OutputDebugStringf("Vertex Dimensions: %d %d\n", vertex_width, vertex_length);
	float actual_terrain_width = (float)(vertex_width - 1) * terrain_cell_length;
	float actual_terrain_length = (float)(vertex_length - 1) * terrain_cell_length;
	Vector3 O(-actual_terrain_width / 2.0f, 0.0f, actual_terrain_length / 2.0f);

	//Compute terrain vertices
	terrain.mesh.vertices = (Mesh_vertex*)alloc_mem(terrain.mesh.number_of_vertices * sizeof(Mesh_vertex));
	for (int i = 0; i < vertex_length; ++i)
	{
		for (int j = 0; j < vertex_width; ++j)
		{
			float v_x = (float)j * terrain_cell_length;
			float v_z = (float)i * terrain_cell_length;
			terrain.mesh.vertices[i*vertex_width + j].position = O + Vector3(v_x, 0.0f, -v_z);
			terrain.mesh.vertices[i*vertex_width + j].normal = Vector3(0.f, 1.0f, 0.0f);

			float y_0 = 1.0f * terrain.perlin_noise(0.5f * v_x, 0.5f * v_z);
			float y_1 = 0.5f * terrain.perlin_noise(2.0f * v_x, 2.0f * v_z);
			float y_2 = 0.25f * terrain.perlin_noise(4.0f * v_x, 4.0f * v_z);
			float y = normalise_range(y_0 + y_1 + y_2);

			float v_y = (y >= 0.0f) ? pow(y, 3.0f) : 0.0f;
			if (v_y > terrain.max_height) terrain.max_height = v_y;
			terrain.mesh.vertices[i*vertex_width + j].position.y = v_y;
		}
	}

	//Number of indices = indices per triangle * triangles per rectangle * number of rectangles in grid
	int number_of_rectangles = (vertex_width - 1)*(vertex_length - 1);
	int number_of_triangles = 2 * number_of_rectangles;
	terrain.mesh.number_of_indices = 3 * number_of_triangles;
	terrain.mesh.face_indices = (GLuint*)alloc_mem(3 * number_of_triangles * sizeof(GLuint));

	//Compute indices
	for (int i = 0; i < number_of_rectangles; ++i)
	{
		GLuint r_0 = (i / (vertex_width - 1)) * vertex_width + (i % (vertex_width - 1));
		GLuint r_1 = r_0 + 1;
		GLuint r_2 = r_1 + vertex_width;
		GLuint r_3 = r_2 - 1;

		GLuint rect_indices[] = { r_0, r_1, r_2, r_2, r_3, r_0 };
		for (int j = 0; j < 6; ++j)
		{
			terrain.mesh.face_indices[6 * i + j] = rect_indices[j];
		}
	}

	terrain.mesh.other_edges = (int*)alloc_mem(terrain.mesh.number_of_indices * sizeof(int));
	terrain.mesh.first_edges = (int*)alloc_mem(terrain.mesh.number_of_vertices * sizeof(int));
	terrain.mesh.compute_first_edge_list();
	terrain.mesh.compute_other_edge_list();
	terrain.mesh.compute_normal_vectors();

	terrain.graphical_data = buffer_mesh(terrain.mesh);

	return terrain;
}
