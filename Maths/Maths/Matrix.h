/*
Group Project
Game Engine Maths Library
Matrix.h
*/

#pragma once
class Matrix
{
public:
	// Constructors
	Matrix();
	Matrix(float value);
	Matrix(float aa, float ab, float ac, float ad, float ba, float bb, float bc, float bd, float ca, float cb, float cc, float cd, float da, float db, float dc, float dd);
	Matrix(Matrix* matrix);

	// Setters
	void setaa(float value);
	void setab(float value);
	void setac(float value);
	void setad(float value);
	void setba(float value);
	void setbb(float value);
	void setbc(float value);
	void setbd(float value);
	void setca(float value);
	void setcb(float value);
	void setcc(float value);
	void setcd(float value);
	void setda(float value);
	void setdb(float value);
	void setdc(float value);
	void setdd(float value);

	// Getters
	float getaa();
	float getab();
	float getac();
	float getad();
	float getba();
	float getbb();
	float getbc();
	float getbd();
	float getca();
	float getcb();
	float getcc();
	float getcd();
	float getda();
	float getdb();
	float getdc();
	float getdd();

	// Overloading operators
	Matrix operator +(Matrix matrix);
	Matrix operator -(Matrix matrix);
	Matrix operator *(float scalar);
	Matrix operator *(Matrix matrix);
	Matrix operator /(float scalar);

	// Methods
	Matrix negative();
	Matrix transpose();

private:
	// Matrix values
	float values[4][4];
};

