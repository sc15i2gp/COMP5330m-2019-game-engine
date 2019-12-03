#include "Matrix.h"

Matrix::Matrix() {
	values[0][0] = values[0][1] = values[0][2]
		= values[1][0] = values[1][1] = values[1][2]
		= values[2][0] = values[2][1] = values[2][2] = 0.0;
}

Matrix::Matrix(float value) {
	values[0][0] = values[1][1] = values[2][2] = value;
	values[0][1] = values[0][2] = values[1][0]
		= values[1][2] = values[2][0] = values[2][1] = 0.0;
}

Matrix::Matrix(float aa, float ab, float ac, float ba, float bb, float bc, float ca, float cb, float cc) {
	values[0][0] = aa;
	values[0][1] = ab;
	values[0][2] = ac;
	values[1][0] = ba;
	values[1][1] = bb;
	values[1][2] = bc;
	values[2][0] = ca;
	values[2][1] = cb;
	values[2][2] = cc;
}

void Matrix::setaa(float value) {
	values[0][0] = value;
}

void Matrix::setab(float value) {
	values[0][1] = value;
}

void Matrix::setac(float value) {
	values[0][2] = value;
}

void Matrix::setba(float value) {
	values[1][0] = value;
}

void Matrix::setbb(float value) {
	values[1][1] = value;
}

void Matrix::setbc(float value) {
	values[1][2] = value;
}

void Matrix::setca(float value) {
	values[2][0] = value;
}

void Matrix::setcb(float value) {
	values[2][1] = value;
}

void Matrix::setcc(float value) {
	values[2][2] = value;
}

float Matrix::getaa() {
	return values[0][0];
}

float Matrix::getab() {
	return values[0][1];
}

float Matrix::getac() {
	return values[0][2];
}

float Matrix::getba() {
	return values[1][0];
}

float Matrix::getbb() {
	return values[1][1];
}

float Matrix::getbc() {
	return values[1][2];
}

float Matrix::getca() {
	return values[2][0];
}

float Matrix::getcb() {
	return values[2][1];
}

float Matrix::getcc() {
	return values[2][2];
}

Matrix Matrix::operator+(Matrix matrix) {
	Matrix result(values[0][0] + matrix.values[0][0],
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

Matrix Matrix::operator-(Matrix matrix) {
	Matrix result(values[0][0] - matrix.values[0][0],
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

Matrix Matrix::operator*(float scalar) {
	Matrix result(values[0][0] * scalar,
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

Matrix Matrix::operator*(Vector vector) {
	Matrix result(values[0][0] * vector.getX(),
		values[0][1] * vector.getY(),
		values[0][2] * vector.getZ(),
		values[1][0] * vector.getX(),
		values[1][1] * vector.getY(),
		values[1][2] * vector.getZ(),
		values[2][0] * vector.getX(),
		values[2][1] * vector.getY(),
		values[2][2] * vector.getZ());
	return result;
}
