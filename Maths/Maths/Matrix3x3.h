#/*
Group Project
Game Engine Maths Library
Matrix3x3.h
*/

#include "Vector3.h"

#pragma once
class Matrix3x3
{
public:
	// Constructors
	Matrix3x3();
	Matrix3x3(float value);
	Matrix3x3(float aa, float ab, float ac, float ba, float bb, float bc, float ca, float cb, float cc);
	Matrix3x3(Matrix3x3* matrix);

	// Setters
	void setaa(float value);
	void setab(float value);
	void setac(float value);
	void setba(float value);
	void setbb(float value);
	void setbc(float value);
	void setca(float value);
	void setcb(float value);
	void setcc(float value);

	// Getters
	float getaa();
	float getab();
	float getac();
	float getba();
	float getbb();
	float getbc();
	float getca();
	float getcb();
	float getcc();

	// Overloading operators
	Matrix3x3 operator +(Matrix3x3 matrix);
	Matrix3x3 operator -(Matrix3x3 matrix);
	Matrix3x3 operator *(float scalar);
	Vector3 operator *(Vector3 vector);
	Matrix3x3 operator *(Matrix3x3 matrix);
	Matrix3x3 operator /(float scalar);

	// Methods
	Matrix3x3 negative();
	Matrix3x3 transpose();

private:
	// Matrix values
	float values[3][3];
};
