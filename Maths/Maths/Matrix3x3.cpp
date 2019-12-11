/*
Group Project
Game Engine Maths Library
Matrix3x3.cpp
*/

#include "Matrix3x3.h"

// Zero matrix
Matrix3x3::Matrix3x3() {
	values[0][0] = values[0][1] = values[0][2]
		= values[1][0] = values[1][1] = values[1][2]
		= values[2][0] = values[2][1] = values[2][2] = 0.0;
}

// Identity matrix with the diagonal values equal to 'value'
Matrix3x3::Matrix3x3(float value) {
	values[0][0] = values[1][1] = values[2][2] = value;
	values[0][1] = values[0][2]
		= values[1][0] = values[1][2]
		= values[2][0] = values[2][1] = 0.0;
}

// Matrix of specified values
Matrix3x3::Matrix3x3(float aa, float ab, float ac, float ba, float bb, float bc, float ca, float cb, float cc) {
	values[0][0] = aa;
	values[1][0] = ba;
	values[2][0] = ca;
	values[0][1] = ab;
	values[1][1] = bb;
	values[2][1] = cb;
	values[0][2] = ac;
	values[1][2] = bc;
	values[2][2] = cc;
}

// Matrix created with another matrix
Matrix3x3::Matrix3x3(Matrix3x3* matrix) {
	values[0][0] = matrix->values[0][0];
	values[1][0] = matrix->values[1][0];
	values[2][0] = matrix->values[2][0];
	values[0][1] = matrix->values[0][1];
	values[1][1] = matrix->values[1][1];
	values[2][1] = matrix->values[2][1];
	values[0][2] = matrix->values[0][2];
	values[1][2] = matrix->values[1][2];
	values[2][2] = matrix->values[2][2];
	values[3][2] = matrix->values[3][2];
}

// Setters
void Matrix3x3::setaa(float value) {
	values[0][0] = value;
}

void Matrix3x3::setab(float value) {
	values[0][1] = value;
}

void Matrix3x3::setac(float value) {
	values[0][2] = value;
}

void Matrix3x3::setba(float value) {
	values[1][0] = value;
}

void Matrix3x3::setbb(float value) {
	values[1][1] = value;
}

void Matrix3x3::setbc(float value) {
	values[1][2] = value;
}

void Matrix3x3::setca(float value) {
	values[2][0] = value;
}

void Matrix3x3::setcb(float value) {
	values[2][1] = value;
}

void Matrix3x3::setcc(float value) {
	values[2][2] = value;
}

// Getters
float Matrix3x3::getaa() {
	return values[0][0];
}

float Matrix3x3::getab() {
	return values[0][1];
}

float Matrix3x3::getac() {
	return values[0][2];
}

float Matrix3x3::getba() {
	return values[1][0];
}

float Matrix3x3::getbb() {
	return values[1][1];
}

float Matrix3x3::getbc() {
	return values[1][2];
}

float Matrix3x3::getca() {
	return values[2][0];
}

float Matrix3x3::getcb() {
	return values[2][1];
}

float Matrix3x3::getcc() {
	return values[2][2];
}

// Add two matrices together
Matrix3x3 Matrix3x3::operator+(Matrix3x3 matrix) {
	Matrix3x3 result(values[0][0] + matrix.values[0][0],
		values[0][1] + matrix.values[0][1],
		values[0][2] + matrix.values[0][2],
		values[1][0] + matrix.values[1][0],
		values[1][1] + matrix.values[1][1],
		values[1][2] + matrix.values[1][2],
		values[2][0] + matrix.values[2][0],
		values[2][1] + matrix.values[2][1],
		values[2][2] + matrix.values[2][2]);
	return result;
}

// Subtract a matrix by the other matrix
Matrix3x3 Matrix3x3::operator-(Matrix3x3 matrix) {
	Matrix3x3 result(values[0][0] - matrix.values[0][0],
		values[0][1] - matrix.values[0][1],
		values[0][2] - matrix.values[0][2],
		values[1][0] - matrix.values[1][0],
		values[1][1] - matrix.values[1][1],
		values[1][2] - matrix.values[1][2],
		values[2][0] - matrix.values[2][0],
		values[2][1] - matrix.values[2][1],
		values[2][2] - matrix.values[2][2]);
	return result;
}

// Scalar multiplication
Matrix3x3 Matrix3x3::operator*(float scalar) {
	Matrix3x3 result(values[0][0] * scalar,
		values[0][1] * scalar,
		values[0][2] * scalar,
		values[1][0] * scalar,
		values[1][1] * scalar,
		values[1][2] * scalar,
		values[2][0] * scalar,
		values[2][1] * scalar,
		values[2][2] * scalar);
	return result;
}

// Multiplication of matrix and vector
Vector3 Matrix3x3::operator*(Vector3 vector) {
	Vector3 result((values[0][0] * vector.getX()) + (values[0][1] * vector.getY()) + (values[0][2] * vector.getZ()),
		(values[1][0] * vector.getX()) + (values[1][1] * vector.getY()) + (values[1][2] * vector.getZ()),
		(values[2][0] * vector.getX()) + (values[2][1] * vector.getY()) + (values[2][2] * vector.getZ()));
	return result;
}

// Matrix multiplication
Matrix3x3 Matrix3x3::operator*(Matrix3x3 matrix) {
	Matrix3x3 result((values[0][0] * matrix.values[0][0]) + (values[0][1] * matrix.values[1][0]) + (values[0][2] * matrix.values[2][0]),
		(values[0][0] * matrix.values[0][1]) + (values[0][1] * matrix.values[1][1]) + (values[0][2] * matrix.values[2][1]),
		(values[0][0] * matrix.values[0][2]) + (values[0][1] * matrix.values[1][2]) + (values[0][2] * matrix.values[2][2]),
		(values[1][0] * matrix.values[0][0]) + (values[1][1] * matrix.values[1][0]) + (values[1][2] * matrix.values[2][0]),
		(values[1][0] * matrix.values[0][1]) + (values[1][1] * matrix.values[1][1]) + (values[1][2] * matrix.values[2][1]),
		(values[1][0] * matrix.values[0][2]) + (values[1][1] * matrix.values[1][2]) + (values[1][2] * matrix.values[2][2]),
		(values[2][0] * matrix.values[0][0]) + (values[2][1] * matrix.values[1][0]) + (values[2][2] * matrix.values[2][0]),
		(values[2][0] * matrix.values[0][1]) + (values[2][1] * matrix.values[1][1]) + (values[2][2] * matrix.values[2][1]),
		(values[2][0] * matrix.values[0][2]) + (values[2][1] * matrix.values[1][2]) + (values[2][2] * matrix.values[2][2]));
	return result;
}

// Scalar division
Matrix3x3 Matrix3x3::operator/(float scalar) {
	Matrix3x3 result(values[0][0] / scalar,
		values[0][1] / scalar,
		values[0][2] / scalar,
		values[1][0] / scalar,
		values[1][1] / scalar,
		values[1][2] / scalar,
		values[2][0] / scalar,
		values[2][1] / scalar,
		values[2][2] / scalar);
	return result;
}

// Return a matrix where each value is negated
Matrix3x3 Matrix3x3::negative() {
	Matrix3x3 negative(-values[0][0], -values[0][1], -values[0][2],
		-values[1][0], -values[1][1], -values[1][2],
		-values[2][0], -values[2][1], -values[2][2]);
	return negative;
}

// Transposed matrix
Matrix3x3 Matrix3x3::transpose() {
	Matrix3x3 transpose(values[0][0], values[1][0], values[2][0],
		values[0][1], values[1][1], values[2][1],
		values[0][2], values[1][2], values[2][2]);
	return transpose;
}
