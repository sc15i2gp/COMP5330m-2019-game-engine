#pragma once
#include "Maths.h"
#include "Platform.h"

#define NUMBER_OF_NODES 256

class Perlin_Noise_Function
{
public:
	static Perlin_Noise_Function __generate();

	float operator()(float x, float y);

private:
	Vector2 gradient_at_node(int x, int y);
	float interpolate(float[4], Vector2);

	Vector2 nodes[NUMBER_OF_NODES];
};

#define generate_noise_function() Perlin_Noise_Function::__generate()