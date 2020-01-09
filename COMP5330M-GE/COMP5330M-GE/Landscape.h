#pragma once
#include "Perlin_Noise.h"
#include "Graphics.h"

//Goal: Render a procedurally generated landscape
//Landscape:
//	- Terrain
//	- Trees

//Terrain:
//	- Load an array of regularly spaced vertices in a grid
//	- Use perlin noise to set their y values

struct Terrain
{
	Drawable graphical_data;
	float max_height;
	float width;
	float length;
};

Terrain create_terrain(float width, float length, float cell_length = 10.0f);