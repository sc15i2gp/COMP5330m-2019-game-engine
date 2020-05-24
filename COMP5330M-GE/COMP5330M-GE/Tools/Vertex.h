#pragma once

#include "Maths.h"
#include "Rendering/RGBAColour.h"

struct Vertex
{

	Vector3 position;
	RGBAColour colour;

	Vertex(const Vector3& pos, const RGBAColour& colour);
};

