#pragma once
class Vector
{
public:
	Vector();
	Vector(float x, float y, float z);
	Vector(Vector *vector);

	void setX(float value);
	void setY(float value);
	void setZ(float value);

	float getX();
	float getY();
	float getZ();

	Vector operator +(Vector vector);
	Vector operator -(Vector vector);
	Vector operator *(float value);
	float operator *(Vector vector);
	Vector operator /(float value);

	float length();
	Vector normalize();
	Vector cross(Vector vector);
	
private:
	float x;
	float y;
	float z;
};

