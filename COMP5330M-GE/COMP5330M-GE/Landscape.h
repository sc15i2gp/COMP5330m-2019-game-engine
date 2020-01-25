#pragma once
#include "Perlin_Noise.h"
#include "Graphics.h"
#include "Maths.h"

//Goal: Render a procedurally generated landscape
//Landscape:
//	- Terrain
//	- Trees

//Terrain:
//	- Load an array of regularly spaced vertices in a grid
//	- Use perlin noise to set their y values

struct Terrain
{
	Perlin_Noise_Function perlin_noise;
	Drawable graphical_data;
	Mesh mesh;
	float max_height;
	float width;
	float length;
};

Terrain create_terrain(float width, float length, float terrain_cell_length, float frequency = 1.0f);