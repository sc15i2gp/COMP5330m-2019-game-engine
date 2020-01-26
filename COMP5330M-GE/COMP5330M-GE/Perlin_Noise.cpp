#include "Perlin_Noise.h"

float random(float min = -1.0f, float max = 1.0f)
{
	float n = (float)rand() / (float)RAND_MAX;
	float range = max - min;
	return min + n * range;
}

Vector2 rand_vector2(float min = -1.0f, float max = 1.0f)
{
	Vector2 v = {};
	while (true)
	{
		v.x = random();
		v.y = random();
		if (length(v) <= 1.0 && length(v) >= 0.1) break;
	}
	return v;
}


Perlin_Noise_Function Perlin_Noise_Function::__generate()
{
	Perlin_Noise_Function func = {};
	srand(NULL);
	for (int i = 0; i < NUMBER_OF_NODES; ++i)
	{
		func.nodes[i] = normalise(rand_vector2());
	}

	return func;
}

Vector2 Perlin_Noise_Function::gradient_at_node(int x, int y)
{
	//Hash x and y
	//Return gradient in that index
	return this->nodes[GRID_LENGTH*y + x];
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
	return linear_interpolation(i_0, i_1, weight[1]);
}

float fade(float f)
{
	return f * f * f * (f * (f * 6.0 - 15.0) + 10.0f);
}

//x,y will be given between 0 and terrain_width/length
//Returns value approximately between -1 and 1
float Perlin_Noise_Function::operator()(float x, float y)
{
	int cell_x = floor(x);
	int cell_y = floor(y);

	//Find the 4 distances between (x, y) and the grid nodes within which it falls
	int cell_nodes[4][2] =
	{
		{cell_x, cell_y}, //Bottom left
		{cell_x + 1, cell_y}, //Bottom right
		{cell_x, cell_y + 1}, //Top left
		{cell_x + 1, cell_y + 1} //Top right
	};
	Vector2 gradients[4] = {};
	Vector2 distances[4] = {};

	for (int i = 0; i < 4; ++i) gradients[i] = this->gradient_at_node(cell_nodes[i][0], cell_nodes[i][1]);
	for (int i = 0; i < 4; ++i) distances[i] = Vector2{ x, y } -Vector2{ (float)cell_nodes[i][0], (float)cell_nodes[i][1] };
	//Find the dot products between the distance and grid gradient vectors
	float dot_products[4] = {};
	for (int i = 0; i < 4; ++i)
	{
		dot_products[i] = dot(gradients[i], distances[i]);
	}

	//Return value linearly interpolated between dot products
	Vector2 weight = {};
	weight[0] = fade(x - (float)cell_x);
	weight[1] = fade(y - (float)cell_y);

	float noise_val = interpolate(dot_products, weight);
	return noise_val;
}