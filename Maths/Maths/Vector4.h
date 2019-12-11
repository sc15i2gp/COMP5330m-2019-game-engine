/*
Group Project
Game Engine Maths Library
Vector4.h
*/

#include "Vector3.h"

#pragma once
class Vector4
{
public:
	// Constructors
	Vector4();
	Vector4(float x, float y, float z, float w);
	Vector4(Vector4* vector);
	Vector4(Vector3 vector, float w);

	// Setters
	void setX(float value);
	void setY(float value);
	void setZ(float value);
	void setW(float value);

	// Getters
	float getX();
	float getY();
	float getZ();
	float getW();

	// Overloading operators
	Vector4 operator +(Vector4 vector);
	Vector4 operator -(Vector4 vector);
	Vector4 operator *(float value);
	float operator *(Vector4 vector);
	Vector4 operator /(float value);

	// Methods
	float length();
	Vector4 normalize();

private:
	// Values
	float x;
	float y;
	float z;
	float w;
};
