/*
Group Project
Game Engine Maths Library
Maths.cpp
*/

#include "Vector.h"
#include <cmath>

// A vector of (0,0,0)
Vector::Vector() {
	x = 0;
	y = 0;
	z = 0;
}

// A vector of (x,y,z)
Vector::Vector(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

// A vector constructed of another vector
Vector::Vector(Vector* vector) {
	x = vector->x;
	y = vector->y;
	z = vector->z;
}

// Set the value of x
void Vector::setX(float value) {
	x = value;
}

// Set the value of y
void Vector::setY(float value) {
	y = value;
}

// Set the value of z
void Vector::setZ(float value) {
	z = value;
}

// Get the value of x
float Vector::getX() {
	return x;
}

// Get the value of y
float Vector::getY() {
	return y;
}

// Get the value of z
float Vector::getZ() {
	return z;
}

// Add two vertices together
Vector Vector::operator+(Vector vector) {
	Vector result(x + vector.x, y + vector.y, z + vector.z);
	return result;
}

// Subtract one vertex from another
Vector Vector::operator-(Vector vector) {
	Vector result(x - vector.x, y - vector.y, z - vector.z);
	return result;
}

// Multiply a vertex by a scalar
Vector Vector::operator*(float value) {
	Vector result(x * value, y * value, z * value);
	return result;
}

// Dot product of two vertices
float Vector::operator*(Vector vector) {
	float result = (x * vector.x) + (y * vector.y) + (z * vector.z);
	return result;
}

// Divide a vertex by a scalar
Vector Vector::operator/(float value) {
	Vector result(x / value, y / value, z / value);
	return result;
}

// Return the length of the vertex
float Vector::length() {
	float length = sqrt((x * x) + (y * y) + (z * z));
	return length;
}

// Get the same vector with the length of 1
Vector Vector::normalize() {
	float length = sqrt((x * x) + (y * y) + (z * z));
	Vector normalized(x / length, y / length, z / length);
	return normalized;
}

// Cross product of two vertices
Vector Vector::cross(Vector vector) {
	Vector result(y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y * vector.x);
	return result;
}
