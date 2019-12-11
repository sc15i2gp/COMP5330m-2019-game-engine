/*
Group Project
Game Engine Maths Library
Matrix4x4.cpp
*/

#include "Matrix4x4.h"

// Zero matrix
Matrix4x4::Matrix4x4() {
	values[0][0] = values[0][1] = values[0][2] = values[0][3]
		= values[1][0] = values[1][1] = values[1][2] = values[1][3]
		= values[2][0] = values[2][1] = values[2][2] = values[2][3]
		= values[3][0] = values[3][1] = values[3][2] = values[3][3] = 0.0;
}

// Identity matrix with the diagonal values equal to 'value'
Matrix4x4::Matrix4x4(float value) {
	values[0][0] = values[1][1] = values[2][2] = values[3][3] = value;
	values[0][1] = values[0][2] = values[0][3]
		= values[1][0] = values[1][2] = values[1][3] 
		= values[2][0] = values[2][1] = values[2][3]
		= values[3][0] = values[3][1] = values[3][2] = 0.0;
}

// Matrix of specified values
Matrix4x4::Matrix4x4(float aa, float ab, float ac, float ad, float ba, float bb, float bc, float bd, float ca, float cb, float cc, float cd, float da, float db, float dc, float dd) {
	values[0][0] = aa;
	values[1][0] = ba;
	values[2][0] = ca;
	values[3][0] = da;
	values[0][1] = ab;
	values[1][1] = bb;
	values[2][1] = cb;
	values[3][1] = db;
	values[0][2] = ac;
	values[1][2] = bc;
	values[2][2] = cc;
	values[3][2] = dc;
	values[0][3] = ad;
	values[1][3] = bd;
	values[2][3] = cd;
	values[3][3] = dd;
}

// Matrix created with another matrix
Matrix4x4::Matrix4x4(Matrix4x4* matrix) {
	values[0][0] = matrix->values[0][0];
	values[1][0] = matrix->values[1][0];
	values[2][0] = matrix->values[2][0];
	values[3][0] = matrix->values[3][0];
	values[0][1] = matrix->values[0][1];
	values[1][1] = matrix->values[1][1];
	values[2][1] = matrix->values[2][1];
	values[3][1] = matrix->values[3][1];
	values[0][2] = matrix->values[0][2];
	values[1][2] = matrix->values[1][2];
	values[2][2] = matrix->values[2][2];
	values[3][2] = matrix->values[3][2];
	values[0][3] = matrix->values[0][3];
	values[1][3] = matrix->values[1][3];
	values[2][3] = matrix->values[2][3];
	values[3][3] = matrix->values[3][3];
}

// Setters
void Matrix4x4::setaa(float value) {
	values[0][0] = value;
}

void Matrix4x4::setab(float value) {
	values[0][1] = value;
}

void Matrix4x4::setac(float value) {
	values[0][2] = value;
}

void Matrix4x4::setad(float value) {
	values[0][3] = value;
}

void Matrix4x4::setba(float value) {
	values[1][0] = value;
}

void Matrix4x4::setbb(float value) {
	values[1][1] = value;
}

void Matrix4x4::setbc(float value) {
	values[1][2] = value;
}

void Matrix4x4::setbd(float value) {
	values[1][3] = value;
}

void Matrix4x4::setca(float value) {
	values[2][0] = value;
}

void Matrix4x4::setcb(float value) {
	values[2][1] = value;
}

void Matrix4x4::setcc(float value) {
	values[2][2] = value;
}

void Matrix4x4::setcd(float value) {
	values[2][3] = value;
}

void Matrix4x4::setda(float value) {
	values[3][0] = value;
}

void Matrix4x4::setdb(float value) {
	values[3][1] = value;
}

void Matrix4x4::setdc(float value) {
	values[3][2] = value;
}

void Matrix4x4::setdd(float value) {
	values[3][3] = value;
}

// Getters
float Matrix4x4::getaa() {
	return values[0][0];
}

float Matrix4x4::getab() {
	return values[0][1];
}

float Matrix4x4::getac() {
	return values[0][2];
}

float Matrix4x4::getad() {
	return values[0][3];
}

float Matrix4x4::getba() {
	return values[1][0];
}

float Matrix4x4::getbb() {
	return values[1][1];
}

float Matrix4x4::getbc() {
	return values[1][2];
}

float Matrix4x4::getbd() {
	return values[1][3];
}

float Matrix4x4::getca() {
	return values[2][0];
}

float Matrix4x4::getcb() {
	return values[2][1];
}

float Matrix4x4::getcc() {
	return values[2][2];
}

float Matrix4x4::getcd() {
	return values[2][3];
}

float Matrix4x4::getda() {
	return values[3][0];
}

float Matrix4x4::getdb() {
	return values[3][1];
}

float Matrix4x4::getdc() {
	return values[3][2];
}

float Matrix4x4::getdd() {
	return values[3][3];
}

// Add two matrices together
Matrix4x4 Matrix4x4::operator+(Matrix4x4 matrix) {
	Matrix4x4 result(values[0][0] + matrix.values[0][0],
		values[0][1] + matrix.values[0][1],
		values[0][2] + matrix.values[0][2],
		values[0][3] + matrix.values[0][3],
		values[1][0] + matrix.values[1][0],
		values[1][1] + matrix.values[1][1],
		values[1][2] + matrix.values[1][2],
		values[1][3] + matrix.values[1][3],
		values[2][0] + matrix.values[2][0],
		values[2][1] + matrix.values[2][1],
		values[2][2] + matrix.values[2][2],
		values[2][3] + matrix.values[2][3],
		values[3][0] + matrix.values[3][0],
		values[3][1] + matrix.values[3][1],
		values[3][2] + matrix.values[3][2],
		values[3][3] + matrix.values[3][3]);
	return result;
}

// Subtract a matrix by the other matrix
Matrix4x4 Matrix4x4::operator-(Matrix4x4 matrix) {
	Matrix4x4 result(values[0][0] - matrix.values[0][0],
		values[0][1] - matrix.values[0][1],
		values[0][2] - matrix.values[0][2],
		values[0][3] - matrix.values[0][3],
		values[1][0] - matrix.values[1][0],
		values[1][1] - matrix.values[1][1],
		values[1][2] - matrix.values[1][2],
		values[1][3] - matrix.values[1][3],
		values[2][0] - matrix.values[2][0],
		values[2][1] - matrix.values[2][1],
		values[2][2] - matrix.values[2][2],
		values[2][3] - matrix.values[2][3],
		values[3][0] - matrix.values[3][0],
		values[3][1] - matrix.values[3][1],
		values[3][2] - matrix.values[3][2],
		values[3][3] - matrix.values[3][3]);
	return result;
}

// Scalar multiplication
Matrix4x4 Matrix4x4::operator*(float scalar) {
	Matrix4x4 result(values[0][0] * scalar,
		values[0][1] * scalar,
		values[0][2] * scalar,
		values[0][3] * scalar,
		values[1][0] * scalar,
		values[1][1] * scalar,
		values[1][2] * scalar,
		values[1][3] * scalar,
		values[2][0] * scalar,
		values[2][1] * scalar,
		values[2][2] * scalar,
		values[2][3] * scalar,
		values[3][0] * scalar,
		values[3][1] * scalar,
		values[3][2] * scalar,
		values[3][3] * scalar);
	return result;
}

// Multiplication of matrix and vector
Vector4 Matrix4x4::operator*(Vector4 vector) {
	Vector4 result((values[0][0] * vector.getX()) + (values[0][1] * vector.getY()) + (values[0][2] * vector.getZ()) + (values[0][3] * vector.getW()),
		(values[1][0] * vector.getX()) + (values[1][1] * vector.getY()) + (values[1][2] * vector.getZ()) + (values[1][3] * vector.getW()),
		(values[2][0] * vector.getX()) + (values[2][1] * vector.getY()) + (values[2][2] * vector.getZ()) + (values[2][3] * vector.getW()),
		(values[3][0] * vector.getX()) + (values[3][1] * vector.getY()) + (values[3][2] * vector.getZ()) + (values[3][3] * vector.getW()));
	return result;
}

// Matrix multiplication
Matrix4x4 Matrix4x4::operator*(Matrix4x4 matrix) {
	Matrix4x4 result((values[0][0] * matrix.values[0][0]) + (values[0][1] * matrix.values[1][0]) + (values[0][2] * matrix.values[2][0]) + (values[0][3] * matrix.values[3][0]),
		(values[0][0] * matrix.values[0][1]) + (values[0][1] * matrix.values[1][1]) + (values[0][2] * matrix.values[2][1]) + (values[0][3] * matrix.values[3][1]),
		(values[0][0] * matrix.values[0][2]) + (values[0][1] * matrix.values[1][2]) + (values[0][2] * matrix.values[2][2]) + (values[0][3] * matrix.values[3][2]),
		(values[0][0] * matrix.values[0][3]) + (values[0][1] * matrix.values[1][3]) + (values[0][2] * matrix.values[2][3]) + (values[0][3] * matrix.values[3][3]),
		(values[1][0] * matrix.values[0][0]) + (values[1][1] * matrix.values[1][0]) + (values[1][2] * matrix.values[2][0]) + (values[1][3] * matrix.values[3][0]),
		(values[1][0] * matrix.values[0][1]) + (values[1][1] * matrix.values[1][1]) + (values[1][2] * matrix.values[2][1]) + (values[1][3] * matrix.values[3][1]),
		(values[1][0] * matrix.values[0][2]) + (values[1][1] * matrix.values[1][2]) + (values[1][2] * matrix.values[2][2]) + (values[1][3] * matrix.values[3][2]),
		(values[1][0] * matrix.values[0][3]) + (values[1][1] * matrix.values[1][3]) + (values[1][2] * matrix.values[2][3]) + (values[1][3] * matrix.values[3][3]),
		(values[2][0] * matrix.values[0][0]) + (values[2][1] * matrix.values[1][0]) + (values[2][2] * matrix.values[2][0]) + (values[2][3] * matrix.values[3][0]),
		(values[2][0] * matrix.values[0][1]) + (values[2][1] * matrix.values[1][1]) + (values[2][2] * matrix.values[2][1]) + (values[2][3] * matrix.values[3][1]),
		(values[2][0] * matrix.values[0][2]) + (values[2][1] * matrix.values[1][2]) + (values[2][2] * matrix.values[2][2]) + (values[2][3] * matrix.values[3][2]),
		(values[2][0] * matrix.values[0][3]) + (values[2][1] * matrix.values[1][3]) + (values[2][2] * matrix.values[2][3]) + (values[2][3] * matrix.values[3][3]),
		(values[3][0] * matrix.values[0][0]) + (values[3][1] * matrix.values[1][0]) + (values[3][2] * matrix.values[2][0]) + (values[3][3] * matrix.values[3][0]),
		(values[3][0] * matrix.values[0][1]) + (values[3][1] * matrix.values[1][1]) + (values[3][2] * matrix.values[2][1]) + (values[3][3] * matrix.values[3][1]),
		(values[3][0] * matrix.values[0][2]) + (values[3][1] * matrix.values[1][2]) + (values[3][2] * matrix.values[2][2]) + (values[3][3] * matrix.values[3][2]),
		(values[3][0] * matrix.values[0][3]) + (values[3][1] * matrix.values[1][3]) + (values[3][2] * matrix.values[2][3]) + (values[3][3] * matrix.values[3][3]));
	return result;
}

// Scalar division
Matrix4x4 Matrix4x4::operator/(float scalar) {
	Matrix4x4 result(values[0][0] / scalar,
		values[0][1] / scalar,
		values[0][2] / scalar,
		values[0][3] / scalar,
		values[1][0] / scalar,
		values[1][1] / scalar,
		values[1][2] / scalar,
		values[1][3] / scalar,
		values[2][0] / scalar,
		values[2][1] / scalar,
		values[2][2] / scalar,
		values[2][3] / scalar,
		values[3][0] / scalar,
		values[3][1] / scalar,
		values[3][2] / scalar,
		values[3][3] / scalar);
	return result;
}

// Return a matrix where each value is negated
Matrix4x4 Matrix4x4::negative() {
	Matrix4x4 negative(-values[0][0], -values[0][1], -values[0][2], -values[0][3],
		-values[1][0], -values[1][1], -values[1][2], -values[1][3],
		-values[2][0], -values[2][1], -values[2][2], -values[2][3],
		-values[3][0], -values[3][1], -values[3][2], -values[3][3]);
	return negative;
}

// Transposed matrix
Matrix4x4 Matrix4x4::transpose() {
	Matrix4x4 transpose(values[0][0], values[1][0], values[2][0], values[3][0],
		values[0][1], values[1][1], values[2][1], values[3][1],
		values[0][2], values[1][2], values[2][2], values[3][2],
		values[0][3], values[1][3], values[2][3], values[3][3]);
	return transpose;
}
