#pragma once
#include "Maths.h"
#include "Platform.h"

class Perlin_Noise_Function
{
public:
	static Perlin_Noise_Function __generate(float w, float h, float c_w, float c_h);
	void __destroy();

	float operator()(float x, float y);

private:
	Vector2 gradient_at_node(int x, int y);
	Vector2 node_coords(int x, int y);
	float interpolate(float[4], Vector2);

	Vector2* nodes;
	Vector2 grid_dimensions;
	Vector2 cell_dimensions;
};

#define generate_noise_function(grid_width, grid_height, cell_width, cell_height) Perlin_Noise_Function::__generate(grid_width, grid_height, cell_width, cell_height)
#define destroy_noise_function(func) func.__destroy()