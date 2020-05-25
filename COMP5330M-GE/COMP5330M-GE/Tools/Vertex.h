#pragma once

#include "Maths.h"
#include "Rendering/RGBAColour.h"

struct Vertex
{
	Vector3 m_position;
	RGBAColour m_colour;
	Vector2 m_textureCoordinate;

	Vertex(const Vector3& pos, const RGBAColour& col, const Vector2& tex);
};

