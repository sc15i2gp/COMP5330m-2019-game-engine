/*
Group Project
Game Engine Maths Library
Vector4.cpp
*/

#include "Vector4.h"
#include <cmath>

// A vector of (0,0,0,0)
Vector4::Vector4() {
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

// A vector of (x,y,z,w)
Vector4::Vector4(float x, float y, float z, float w) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

// A vector constructed of another vector
Vector4::Vector4(Vector4* vector) {
	x = vector->x;
	y = vector->y;
	z = vector->z;
	w = vector->w;
}

// A vector constructed from a 3D vector
Vector4::Vector4(Vector3 vector, float w) {
	x = vector.getX();
	y = vector.getY();
	z = vector.getZ();
	this->w = w;
}

// Set the value of x
void Vector4::setX(float value) {
	x = value;
}

// Set the value of y
void Vector4::setY(float value) {
	y = value;
}

// Set the value of z
void Vector4::setZ(float value) {
	z = value;
}

// Set the value of w
void Vector4::setW(float value) {
	w = value;
}

// Get the value of x
float Vector4::getX() {
	return x;
}

// Get the value of y
float Vector4::getY() {
	return y;
}

// Get the value of z
float Vector4::getZ() {
	return z;
}

// Get the value of w
float Vector4::getW() {
	return w;
}

// Add two vertices together
Vector4 Vector4::operator+(Vector4 vector) {
	Vector4 result(x + vector.x, y + vector.y, z + vector.z, w + vector.w);
	return result;
}

// Subtract one vertex from another
Vector4 Vector4::operator-(Vector4 vector) {
	Vector4 result(x - vector.x, y - vector.y, z - vector.z, w - vector.w);
	return result;
}

// Multiply a vertex by a scalar
Vector4 Vector4::operator*(float value) {
	Vector4 result(x * value, y * value, z * value, w * value);
	return result;
}

// Dot product of two vertices
float Vector4::operator*(Vector4 vector) {
	float result = (x * vector.x) + (y * vector.y) + (z * vector.z) + (w * vector.w);
	return result;
}

// Divide a vertex by a scalar
Vector4 Vector4::operator/(float value) {
	Vector4 result(x / value, y / value, z / value, w / value);
	return result;
}

// Return the length of the vertex
float Vector4::length() {
	float length = sqrt((x * x) + (y * y) + (z * z) + (w * w));
	return length;
}

// Get the same vector with the length of 1
Vector4 Vector4::normalize() {
	float length = sqrt((x * x) + (y * y) + (z * z) + (w * w));
	Vector4 normalized(x / length, y / length, z / length, w / length);
	return normalized;
}
