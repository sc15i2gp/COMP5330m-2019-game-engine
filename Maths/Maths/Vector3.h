/*
Group Project
Game Engine Maths Library
Vector3.h
*/

#pragma once
class Vector3
{
public:
	// Constructors
	Vector3();
	Vector3(float x, float y, float z);
	Vector3(Vector3 *vector);

	// Setters
	void setX(float value);
	void setY(float value);
	void setZ(float value);
	
	// Getters
	float getX();
	float getY();
	float getZ();

	// Overloading operators
	Vector3 operator +(Vector3 vector);
	Vector3 operator -(Vector3 vector);
	Vector3 operator *(float value);
	float operator *(Vector3 vector);
	Vector3 operator /(float value);

	// Methods
	float length();
	Vector3 normalize();
	Vector3 cross(Vector3 vector);
	
private:
	// Values
	float x;
	float y;
	float z;
};

