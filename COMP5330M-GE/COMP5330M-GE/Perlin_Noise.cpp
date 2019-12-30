#include "Perlin_Noise.h"

Vector2 rand_vector2(int n = 500)
{
	Vector2 v = {};
	v.x = (rand() % n) - 2*n;
	v.y = (rand() % n) - 2 * n;
	v = normalise(v);
	return v;
}

Perlin_Noise_Function Perlin_Noise_Function::__generate(float grid_width, float grid_height, float cell_width, float cell_height)
{
	int grid_width_in_cells = (int)ceil(grid_width / cell_width);
	int grid_height_in_cells = (int)ceil(grid_height / cell_height);

	int number_of_cells = grid_width_in_cells * grid_height_in_cells;
	int number_of_nodes = (grid_width_in_cells + 1) * (grid_height_in_cells + 1);

	Perlin_Noise_Function func = {};
	func.nodes = (Vector2*)alloc_mem(number_of_nodes * sizeof(Vector2));
	func.grid_dimensions.x = grid_width;
	func.grid_dimensions.y = grid_height;
	func.cell_dimensions.x = cell_width;
	func.cell_dimensions.y = cell_height;

	srand(NULL);
	for (int i = 0; i < number_of_nodes; ++i)
	{
		func.nodes[i] = rand_vector2();
	}

	return func;
}

void Perlin_Noise_Function::__destroy()
{
	dealloc_mem(this->nodes);
}

Vector2 Perlin_Noise_Function::gradient_at_node(int x, int y)
{
	int grid_width_in_nodes = (int)ceil(this->grid_dimensions.x / this->cell_dimensions.x) + 1;
	return this->nodes[y * grid_width_in_nodes + x];
}

Vector2 Perlin_Noise_Function::node_coords(int x, int y)
{
	Vector2 coords = {};
	coords.x = (float)x * this->cell_dimensions.x;
	coords.y = (float)y * this->cell_dimensions.y;
	return coords;
}

float linear_interpolation(float n, float m, float w)
{
	return (1.0f - w)*n + w * m;
}

float Perlin_Noise_Function::interpolate(float values[4], Vector2 weight)
{
	//Interpolate between first 2 values, then final 2, then interpolate those 2 results
	float i_0 = linear_interpolation(values[0], values[1], weight[0]);
	float i_1 = linear_interpolation(values[2], values[3], weight[0]);
	return linear_interpolation(i_1, i_0, weight[1]);
}

//x,y will be given between 0 and terrain_width/length
float Perlin_Noise_Function::operator()(float x, float y)
{
	//Compute grid cell within which (x, y) falls
	int cell_x = x / this->cell_dimensions.x;
	int cell_y = y / this->cell_dimensions.y;

	//Find the 4 distances between (x, y) and the grid nodes within which it falls
	int cell_nodes[4][2] =
	{
		{cell_x, cell_y},
		{cell_x + 1, cell_y},
		{cell_x, cell_y + 1},
		{cell_x + 1, cell_y + 1}
	};
	Vector2 gradients[4] = {};
	Vector2 distances[4] = {};

	for (int i = 0; i < 4; ++i) gradients[i] = this->gradient_at_node(cell_nodes[i][0], cell_nodes[i][1]);
	for (int i = 0; i < 4; ++i) distances[i] = Vector2{ x, y } -node_coords(cell_nodes[i][0], cell_nodes[i][1]);

	gradients[0] = this->gradient_at_node(cell_x, cell_y);
	gradients[1] = this->gradient_at_node(cell_x + 1, cell_y);
	gradients[2] = this->gradient_at_node(cell_x, cell_y + 1);
	gradients[3] = this->gradient_at_node(cell_x + 1, cell_y + 1);

	//Find the dot products between the distance and grid gradient vectors
	float dot_products[4] = {};
	for (int i = 0; i < 4; ++i)
	{
		dot_products[i] = dot(gradients[i], distances[i]);
	}

	//Return value linearly interpolated between dot products
	Vector2 weight = {};
	weight[0] = (x - ((float)(cell_x)*this->cell_dimensions.x)) / this->cell_dimensions.x;
	weight[1] = (y - ((float)(cell_y)*this->cell_dimensions.y)) / this->cell_dimensions.y;

	return interpolate(dot_products, weight);
}