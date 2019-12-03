#include "Vector.h"

#pragma once
class Matrix
{
public:
	Matrix();
	Matrix(float value);
	Matrix(float aa, float ab, float ac, float ba, float bb, float bc, float ca, float cb, float cc);

	void setaa(float value);
	void setab(float value);
	void setac(float value);
	void setba(float value);
	void setbb(float value);
	void setbc(float value);
	void setca(float value);
	void setcb(float value);
	void setcc(float value);

	float getaa();
	float getab();
	float getac();
	float getba();
	float getbb();
	float getbc();
	float getca();
	float getcb();
	float getcc();

	Matrix operator +(Matrix matrix);
	Matrix operator -(Matrix matrix);
	Matrix operator *(float scalar);
	Matrix operator *(Vector vector);
	Matrix operator *(Matrix matrix);
	Matrix operator /(float scalar);
	Matrix operator /(Vector vector);
	Matrix operator /(Matrix matrix);

	void inverse();
	void transpose();

private:
	float values[3][3];
};

