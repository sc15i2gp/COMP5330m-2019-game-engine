#include "Matrix.h"

Matrix::Matrix() {
	values[0][0] = values[1][0] = values[2][0] = values[3][0]
		= values[0][1] = values[1][1] = values[2][1] = values[3][1]
		= values[0][2] = values[1][2] = values[2][2] = values[3][2]
		= values[0][3] = values[1][3] = values[2][3] = values[3][3] = 0.0;
}

Matrix::Matrix(float value) {
	values[0][0] = values[1][1] = values[2][2] = values[3][3] = value;
	values[1][0] = values[2][0] = values[3][0]
		= values[0][1] = values[2][1] = values[3][1] 
		= values[0][2] = values[1][2] = values[3][2]
		= values[0][3] = values[1][3] = values[2][3] = 0.0;
}

Matrix::Matrix(float aa, float ba, float ca, float da, float ab, float bb, float cb, float db, float ac, float bc, float cc, float dc, float ad, float bd, float cd, float dd) {
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

void Matrix::setaa(float value) {
	values[0][0] = value;
}

void Matrix::setba(float value) {
	values[1][0] = value;
}

void Matrix::setca(float value) {
	values[2][0] = value;
}

void Matrix::setda(float value) {
	values[3][0] = value;
}

void Matrix::setab(float value) {
	values[0][1] = value;
}

void Matrix::setbb(float value) {
	values[1][1] = value;
}

void Matrix::setcb(float value) {
	values[2][1] = value;
}

void Matrix::setdb(float value) {
	values[3][1] = value;
}

void Matrix::setac(float value) {
	values[0][2] = value;
}

void Matrix::setbc(float value) {
	values[1][2] = value;
}

void Matrix::setcc(float value) {
	values[2][2] = value;
}

void Matrix::setdc(float value) {
	values[3][2] = value;
}

void Matrix::setad(float value) {
	values[0][3] = value;
}

void Matrix::setbd(float value) {
	values[1][3] = value;
}

void Matrix::setcd(float value) {
	values[2][3] = value;
}

void Matrix::setdd(float value) {
	values[3][3] = value;
}

float Matrix::getaa() {
	return values[0][0];
}

float Matrix::getba() {
	return values[1][0];
}

float Matrix::getca() {
	return values[2][0];
}

float Matrix::getda() {
	return values[3][0];
}

float Matrix::getab() {
	return values[0][1];
}

float Matrix::getbb() {
	return values[1][1];
}

float Matrix::getcb() {
	return values[2][1];
}

float Matrix::getdb() {
	return values[3][1];
}

float Matrix::getac() {
	return values[0][2];
}

float Matrix::getbc() {
	return values[1][2];
}

float Matrix::getcc() {
	return values[2][2];
}

float Matrix::getdc() {
	return values[3][2];
}

float Matrix::getad() {
	return values[0][3];
}

float Matrix::getbd() {
	return values[1][3];
}

float Matrix::getcd() {
	return values[2][3];
}

float Matrix::getdd() {
	return values[3][3];
}

Matrix Matrix::operator+(Matrix matrix) {
	Matrix result(values[0][0] + matrix.values[0][0],
		values[1][0] + matrix.values[1][0],
		values[2][0] + matrix.values[2][0],
		values[3][0] + matrix.values[3][0],
		values[0][1] + matrix.values[0][1],
		values[1][1] + matrix.values[1][1],
		values[2][1] + matrix.values[2][1],
		values[3][1] + matrix.values[3][1],
		values[0][2] + matrix.values[0][2],
		values[1][2] + matrix.values[1][2],
		values[2][2] + matrix.values[2][2],
		values[3][2] + matrix.values[3][2],
		values[0][3] + matrix.values[0][3],
		values[1][3] + matrix.values[1][3],
		values[2][3] + matrix.values[2][3],
		values[3][3] + matrix.values[3][3]);
	return result;
}

Matrix Matrix::operator-(Matrix matrix) {
	Matrix result(values[0][0] - matrix.values[0][0],
		values[1][0] - matrix.values[1][0],
		values[2][0] - matrix.values[2][0],
		values[3][0] - matrix.values[3][0],
		values[0][1] - matrix.values[0][1],
		values[1][1] - matrix.values[1][1],
		values[2][1] - matrix.values[2][1],
		values[3][1] - matrix.values[3][1],
		values[0][2] - matrix.values[0][2],
		values[1][2] - matrix.values[1][2],
		values[2][2] - matrix.values[2][2],
		values[3][2] - matrix.values[3][2],
		values[0][3] - matrix.values[0][3],
		values[1][3] - matrix.values[1][3],
		values[2][3] - matrix.values[2][3],
		values[3][3] - matrix.values[3][3]);
	return result;
}

Matrix Matrix::operator*(float scalar) {
	Matrix result(values[0][0] * scalar,
		values[1][0] * scalar,
		values[2][0] * scalar,
		values[3][0] * scalar,
		values[0][1] * scalar,
		values[1][1] * scalar,
		values[2][1] * scalar,
		values[3][1] * scalar,
		values[0][2] * scalar,
		values[1][2] * scalar,
		values[2][2] * scalar,
		values[3][2] * scalar,
		values[0][3] * scalar,
		values[1][3] * scalar,
		values[2][3] * scalar,
		values[3][3] * scalar);
	return result;
}

Matrix Matrix::operator*(Matrix matrix) {
	Matrix result((values[0][0] * matrix.values[0][0]) + (values[0][1] * matrix.values[1][0]) + (values[0][2] * matrix.values[2][0]) + (values[0][3] * matrix.values[3][0]),
		(values[1][0] * matrix.values[0][0]) + (values[1][1] * matrix.values[1][0]) + (values[1][2] * matrix.values[2][0]) + (values[1][3] * matrix.values[3][0]),
		(values[2][0] * matrix.values[0][0]) + (values[2][1] * matrix.values[1][0]) + (values[2][2] * matrix.values[2][0]) + (values[2][3] * matrix.values[3][0]),
		(values[3][0] * matrix.values[0][0]) + (values[3][1] * matrix.values[1][0]) + (values[3][2] * matrix.values[2][0]) + (values[3][3] * matrix.values[3][0]),
		(values[0][0] * matrix.values[0][1]) + (values[0][1] * matrix.values[1][1]) + (values[0][2] * matrix.values[2][1]) + (values[0][3] * matrix.values[3][1]),
		(values[1][0] * matrix.values[0][1]) + (values[1][1] * matrix.values[1][1]) + (values[1][2] * matrix.values[2][1]) + (values[1][3] * matrix.values[3][1]),
		(values[2][0] * matrix.values[0][1]) + (values[2][1] * matrix.values[1][1]) + (values[2][2] * matrix.values[2][1]) + (values[2][3] * matrix.values[3][1]),
		(values[3][0] * matrix.values[0][1]) + (values[3][1] * matrix.values[1][1]) + (values[3][2] * matrix.values[2][1]) + (values[3][3] * matrix.values[3][1]),
		(values[0][0] * matrix.values[0][2]) + (values[0][1] * matrix.values[1][2]) + (values[0][2] * matrix.values[2][2]) + (values[0][3] * matrix.values[3][2]),
		(values[1][0] * matrix.values[0][2]) + (values[1][1] * matrix.values[1][2]) + (values[1][2] * matrix.values[2][2]) + (values[1][3] * matrix.values[3][2]),
		(values[2][0] * matrix.values[0][2]) + (values[2][1] * matrix.values[1][2]) + (values[2][2] * matrix.values[2][2]) + (values[2][3] * matrix.values[3][2]),
		(values[3][0] * matrix.values[0][2]) + (values[3][1] * matrix.values[1][2]) + (values[3][2] * matrix.values[2][2]) + (values[3][3] * matrix.values[3][2]),
		(values[0][0] * matrix.values[0][3]) + (values[0][1] * matrix.values[1][3]) + (values[0][2] * matrix.values[2][3]) + (values[0][3] * matrix.values[3][3]),
		(values[1][0] * matrix.values[0][3]) + (values[1][1] * matrix.values[1][3]) + (values[1][2] * matrix.values[2][3]) + (values[1][3] * matrix.values[3][3]),
		(values[2][0] * matrix.values[0][3]) + (values[2][1] * matrix.values[1][3]) + (values[2][2] * matrix.values[2][3]) + (values[2][3] * matrix.values[3][3]),
		(values[3][0] * matrix.values[0][3]) + (values[3][1] * matrix.values[1][3]) + (values[3][2] * matrix.values[2][3]) + (values[3][3] * matrix.values[3][3]));
	return result;
}

Matrix Matrix::operator/(float scalar) {
	Matrix result(values[0][0] / scalar,
		values[1][0] / scalar,
		values[2][0] / scalar,
		values[3][0] / scalar,
		values[0][1] / scalar,
		values[1][1] / scalar,
		values[2][1] / scalar,
		values[3][1] / scalar,
		values[0][2] / scalar,
		values[1][2] / scalar,
		values[2][2] / scalar,
		values[3][2] / scalar,
		values[0][3] / scalar,
		values[1][3] / scalar,
		values[2][3] / scalar,
		values[3][3] / scalar);
	return result;
}

Matrix Matrix::negative() {
	Matrix negative(-values[0][0], -values[1][0], -values[2][0], -values[3][0],
		-values[0][1], -values[1][1], -values[2][1], -values[3][1],
		-values[0][2], -values[1][2], -values[2][2], -values[3][2],
		-values[0][3], -values[1][3], -values[2][3], -values[3][3]);
	return negative;
}

Matrix Matrix::transpose() {
	Matrix transpose(values[0][0], values[0][1], values[0][2], values[0][3],
		values[1][0], values[1][1], values[1][2], values[1][3],
		values[2][0], values[2][1], values[2][2], values[2][3],
		values[3][0], values[3][1], values[3][2], values[3][3]);
	return transpose;
}
