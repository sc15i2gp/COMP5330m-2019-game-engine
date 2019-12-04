#include "Vector.h"

#pragma once
class Matrix
{
public:
	Matrix();
	Matrix(float value);
	Matrix(float aa, float ba, float ca, float da, float ab, float bb, float cb, float db, float ac, float bc, float cc, float dc, float ad, float bd, float cd, float dd);

	void setaa(float value);
	void setba(float value);
	void setca(float value);
	void setda(float value);
	void setab(float value);
	void setbb(float value);
	void setcb(float value);
	void setdb(float value);
	void setac(float value);
	void setbc(float value);
	void setcc(float value);
	void setdc(float value);
	void setad(float value);
	void setbd(float value);
	void setcd(float value);
	void setdd(float value);

	float getaa();
	float getba();
	float getca();
	float getda();
	float getab();
	float getbb();
	float getcb();
	float getdb();
	float getac();
	float getbc();
	float getcc();
	float getdc();
	float getad();
	float getbd();
	float getcd();
	float getdd();

	Matrix operator +(Matrix matrix);
	Matrix operator -(Matrix matrix);
	Matrix operator *(float scalar);
	//Matrix operator *(Vector vector);
	Matrix operator *(Matrix matrix);
	Matrix operator /(float scalar);
	//Matrix operator /(Vector vector);

	Matrix negative();
	Matrix transpose();

private:
	float values[4][4];
};

