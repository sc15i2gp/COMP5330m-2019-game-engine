#include "Landscape.h"

Material wood =
{
	Vector3(0.76, 0.6, 0.42),
	Vector3(0.76, 0.6, 0.42),
	Vector3(0.76, 0.6, 0.42),
	12.0
};

Material leaf =
{
	Vector3(0.17568f, 0.71424f, 0.17568f),
	Vector3(0.17568f, 0.71424f, 0.17568f),
	Vector3(0.17568f, 0.71424f, 0.17568f),
	12.0
};

Material ground =
{
	Vector3(0.0215f, 0.1745f, 0.0215f),
	Vector3(0.07568f, 0.61424f, 0.07568f),
	Vector3(0.633f, 0.727811f, 0.633f),
	256.f
};
float normalise_range(float f)
{
	return (f + 1.0f) / 2.0f;
}

//Buffer a regularly spaced grid of vertices, centered around (0, 0, z)
Terrain_Data create_terrain(float terrain_width, float terrain_length, float terrain_cell_length)
{
	Terrain_Data terrain = {};
	terrain.width = terrain_width;
	terrain.length = terrain_length;
	OutputDebugStringf("Terrain Width: %f\nTerrain Length: %f\n", terrain_width, terrain_length);

	terrain.perlin_noise = generate_noise_function();

	int vertex_width = (int)(ceil(terrain_width / terrain_cell_length)) + 1;
	int vertex_length = (int)(ceil(terrain_length / terrain_cell_length)) + 1;

	terrain.mesh.number_of_vertices = vertex_width * vertex_length;

	//Number of indices = indices per triangle * triangles per rectangle * number of rectangles in grid
	int number_of_rectangles = (vertex_width - 1)*(vertex_length - 1);
	int number_of_triangles = 2 * number_of_rectangles;
	terrain.mesh.number_of_indices = 3 * number_of_triangles;

	OutputDebugStringf("Number of vertices in terrain = %d\n", terrain.mesh.number_of_vertices);
	OutputDebugStringf("Vertex Dimensions: %d %d\n", vertex_width, vertex_length);
	float actual_terrain_width = (float)(vertex_width - 1) * terrain_cell_length;
	float actual_terrain_length = (float)(vertex_length - 1) * terrain_cell_length;
	Vector3 O(-actual_terrain_width / 2.0f, 0.0f, actual_terrain_length / 2.0f);

	//Compute terrain vertices
	terrain.mesh.allocate(terrain.mesh.number_of_vertices, terrain.mesh.number_of_indices);
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

	terrain.mesh.compute_first_edge_list();
	terrain.mesh.compute_other_edge_list();
	terrain.mesh.compute_normal_vectors();

	terrain.terrain_drawable = buffer_mesh(terrain.mesh);

	return terrain;
}

const char* axiom = "A";
Forest_Data create_forest(float width, float length, int number_of_trees)
{
	//Generate tree distribution
	Forest_Data forest = {};
	for (int i = 0; i < number_of_trees; ++i)
	{//For each tree
		//Generate a position
		float x = random(1.0f, width - 1.0f);
		float y = random(1.0f, length - 1.0f);
		forest.tree_distribution.landscape_positions[i] = Vector2{ x, y };
		forest.tree_distribution.tree_types[i] = (Tree_Type)(i % TREE_TYPE_MAX);
		forest.tree_distribution.tree_ids[i] = i % MAX_TREE_DRAWABLES;
	}
	forest.tree_distribution.number_of_trees = number_of_trees;

	l_system rowan_l_system = {};
	add_production(&rowan_l_system, "<A>", "F(2.0, 0.01)");
	l_system pine_l_system = {};
	add_production(&pine_l_system, "<A>", "F(0.5, 0.05)");

	int max_number_of_vertices = 1 << 16;
	int max_number_of_indices = 1 << 18;
	Mesh branch_mesh = {};
	Mesh leaf_mesh = {};
	branch_mesh.allocate(max_number_of_vertices, max_number_of_indices);
	leaf_mesh.allocate(max_number_of_vertices, max_number_of_indices);
	tree_mesh_group tree = {};
	tree.branch_mesh = branch_mesh;
	tree.leaf_mesh = leaf_mesh;

	//Generate tree models
	char tree_str[8192] = {};

	//Generate pine models
	for (int i = 0; i < MAX_TREE_DRAWABLES; ++i)
	{
		strcpy(tree_str, axiom);
		derive_str(&pine_l_system, tree_str);
		run_turtle(tree_str, &tree);
		forest.pine_branch_drawables[i] = buffer_mesh(tree.branch_mesh);
		forest.pine_leaf_drawables[i] = buffer_mesh(tree.leaf_mesh);
	}

	//Generate rowan models
	for (int i = 0; i < MAX_TREE_DRAWABLES; ++i)
	{
		strcpy(tree_str, axiom);
		derive_str(&pine_l_system, tree_str);
		run_turtle(tree_str, &tree);
		forest.rowan_branch_drawables[i] = buffer_mesh(tree.branch_mesh);
		forest.rowan_leaf_drawables[i] = buffer_mesh(tree.leaf_mesh);
	}
	
	branch_mesh.deallocate();
	leaf_mesh.deallocate();
	return forest;
}

Landscape_Data create_landscape(float width, float length, float terrain_cell_length, int number_of_trees)
{
	Landscape_Data landscape = {};
	landscape.forest = create_forest(width, length, number_of_trees);
	landscape.terrain = create_terrain(width, length, terrain_cell_length);
	return landscape;
}

void Landscape_Data::draw()
{
	Matrix4x4 model = identity();
	set_model_matrix(model);
	set_material(ground);
	::draw(this->terrain.terrain_drawable);
	Vector3 terrain_center = {};
	terrain_center.x = -this->terrain.width/2.0f;
	terrain_center.z = -this->terrain.length / 2.0f;
	translate(model, terrain_center);
	for (int i = 0; i < this->forest.tree_distribution.number_of_trees; ++i)
	{
		Vector3 tree_position = {};
		tree_position.x = this->forest.tree_distribution.landscape_positions[i][0];
		tree_position.z = this->forest.tree_distribution.landscape_positions[i][1];
		//tree_position.y = normalise_range(this->terrain.perlin_noise(tree_position.x, tree_position.z));
		tree_position.y = this->terrain.height_at_point(tree_position);
		
		Matrix4x4 tree_model_matrix = model;
		translate(tree_model_matrix, tree_position);
		set_model_matrix(tree_model_matrix);
		Tree_Type current_tree_type = this->forest.tree_distribution.tree_types[i];
		int current_tree_id = this->forest.tree_distribution.tree_ids[i];
		if (current_tree_type == TREE_PINE)
		{
			set_material(wood);
			::draw(this->forest.pine_branch_drawables[current_tree_id]);
			set_material(leaf);
			::draw(this->forest.pine_leaf_drawables[current_tree_id]);
		}
		else if (current_tree_type == TREE_ROWAN)
		{
			set_material(wood);
			::draw(this->forest.rowan_branch_drawables[current_tree_id]);
			set_material(leaf);
			::draw(this->forest.rowan_leaf_drawables[current_tree_id]);
		}
	}
}