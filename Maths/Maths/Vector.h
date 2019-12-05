/*
Group Project
Game Engine Maths Library
Vector.cpp
*/

#pragma once
class Vector
{
public:
	// Constructors
	Vector();
	Vector(float x, float y, float z);
	Vector(Vector *vector);

	// Setters
	void setX(float value);
	void setY(float value);
	void setZ(float value);
	
	// Getters
	float getX();
	float getY();
	float getZ();

	// Overloading operators
	Vector operator +(Vector vector);
	Vector operator -(Vector vector);
	Vector operator *(float value);
	float operator *(Vector vector);
	Vector operator /(float value);

	// Methods
	float length();
	Vector normalize();
	Vector cross(Vector vector);
	
private:
	// Values
	float x;
	float y;
	float z;
};

