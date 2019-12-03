#include "Vector.h"
#include <cmath>

Vector::Vector() {
	x = 0;
	y = 0;
	z = 0;
}

Vector::Vector(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector::Vector(Vector* vector) {
	x = vector->x;
	y = vector->y;
	z = vector->z;
}

void Vector::setX(float value) {
	x = value;
}

void Vector::setY(float value) {
	y = value;
}

void Vector::setZ(float value) {
	z = value;
}

float Vector::getX() {
	return x;
}

float Vector::getY() {
	return y;
}

float Vector::getZ() {
	return z;
}

Vector Vector::operator+(Vector vector) {
	Vector result(x + vector.x, y + vector.y, z + vector.z);
	return result;
}

Vector Vector::operator-(Vector vector) {
	Vector result(x - vector.x, y - vector.y, z - vector.z);
	return result;
}

Vector Vector::operator*(float value) {
	Vector result(x * value, y * value, z * value);
	return result;
}

float Vector::operator*(Vector vector) {
	float result = (x * vector.x) + (y * vector.y) + (z * vector.z);
	return result;
}

Vector Vector::operator/(float value) {
	Vector result(x / value, y / value, z / value);
	return result;
}

float Vector::length() {
	float length = sqrt((x * x) + (y * y) + (z * z));
	return length;
}

Vector Vector::normalize() {
	float length = sqrt((x * x) + (y * y) + (z * z));
	Vector normalized(x / length, y / length, z / length);
	return normalized;
}

Vector Vector::cross(Vector vector) {
	Vector result(y * vector.z - z * vector.y, z * vector.x - x * vector.z, x * vector.y - y * vector.x);
	return result;
}
