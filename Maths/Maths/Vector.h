#pragma once
class Vector
{
public:
	Vector();
	Vector(float x, float y, float z);

	void setX(float value);
	void setY(float value);
	void setZ(float value);

	double getX();
	double getY();
	double getZ();

	Vector operator +(Vector* vector);
	Vector operator -(Vector* vector);
	Vector operator *(float value);
	Vector operator *(Vector* vector);
	Vector operator %(Vector* vector);
	Vector operator /(float value);

	float length();
	Vector normalize();
	
private:
	float x;
	float y;
	float z;
};

