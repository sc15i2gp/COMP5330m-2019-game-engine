/*
Group Project
Game Engine Maths Library
Vector3.cpp
*/

#include "Vector3.h"
#include <cmath>

// A vector of (0,0,0)
Vector3::Vector3() {
	x = 0;
	y = 0;
	z = 0;
}

// A vector of (x,y,z)
Vector3::Vector3(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

// A vector constructed of another vector
Vector3::Vector3(Vector3* vector) {
	x = vector->x;
	y = vector->y;
	z = vector->z;
}

// Set the value of x
void Vector3::setX(float value) {
	x = value;
}

// Set the value of y
void Vector3::setY(float value) {
	y = value;
}

// Set the value of z
void Vector3::setZ(float value) {
	z = value;
}

// Get the value of x
float Vector3::getX() {
	return x;
}

// Get the value of y
float Vector3::getY() {
	return y;
}

// Get the value of z
float Vector3::getZ() {
	return z;
}

// Add two vertices together
Vector3 Vector3::operator+(Vector3 vector) {
	Vector3 result(x + vector.x, y + vector.y, z + vector.z);
	return result;
}

// Subtract one vertex from another
Vector3 Vector3::operator-(Vector3 vector) {
	Vector3 result(x - vector.x, y - vector.y, z - vector.z);
	return result;
}

// Multiply a vertex by a scalar
Vector3 Vector3::operator*(float value) {
	Vector3 result(x * value, y * value, z * value);
	return result;
}

// Dot product of two vertices
float Vector3::operator*(Vector3 vector) {
	float result = (x * vector.x) + (y * vector.y) + (z * vector.z);
	return result;
}

// Divide a vertex by a scalar
Vector3 Vector3::operator/(float value) {
	Vector3 result(x / value, y / value, z / value);
	return result;
}

// Return the length of the vertex
float Vector3::length() {
	float length = sqrt((x * x) + (y * y) + (z * z));
	return length;
}

// Get the same vector with the length of 1
Vector3 Vector3::normalize() {
	float length = sqrt((x * x) + (y * y) + (z * z));
	Vector3 normalized(x / length, y / length, z / length);
	return normalized;
}

// Cross product of two vertices
Vector3 Vector3::cross(Vector3 vector) {
	Vector3 result(y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y * vector.x);
	return result;
}
