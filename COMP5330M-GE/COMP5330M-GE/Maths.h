#pragma once
#include <math.h>

//Properties:
//Matrix: Column major, since OpenGL uses column major matrices
//Matrix: All square matrices, if you need rectangular matrices, T.S.
//Matrix[i] returns a reference to the i'th column vector in the matrix
//Matrix.row(i) returns a copy of the i'th row as a column vector

#define PI 3.141592653f

/****************************/

/*			Trig			*/

/****************************/

double cos_deg(double);
double sin_deg(double);
double tan_deg(double);


/****************************/

/*			Vector2			*/

/****************************/

struct Vector2
{
	union
	{
		float xy[2];
		struct
		{
			float x;
			float y;
		};
	};

	float& operator[](int index);
};

Vector2 operator+(Vector2, Vector2);
Vector2 operator-(Vector2, Vector2);
Vector2 operator/(Vector2, float);
Vector2 operator/(Vector2, Vector2); //Pairwise division
Vector2 operator*(float, Vector2);
float dot(Vector2, Vector2);
float length(Vector2);
Vector2 normalise(Vector2);

/****************************/

/*			Vector3			*/

/****************************/

struct Vector3
{
	union
	{
		float  xyz[3];
		struct
		{
			float x;
			float y;
			float z;
		};
		struct
		{
			float r;
			float g;
			float b;
		};
		struct
		{
			Vector2 xy;
		};
	};

	Vector3();
	Vector3(float, float, float);
	Vector3(Vector2, float);

	float& operator[](int index)
	{
		return xyz[index];
	}
};

//Simple arithmetic vector operations
Vector3 operator+(Vector3, Vector3);
Vector3 operator-(Vector3, Vector3);
Vector3 operator*(float, Vector3);
Vector3 operator/(Vector3, float);

void operator+=(Vector3&, Vector3);
void operator-=(Vector3&, Vector3);
void operator*=(Vector3&, float);
void operator/=(Vector3&, float);

Vector3 operator-(Vector3); //Unary

bool operator==(Vector3, Vector3);

//Common vector operations
float dot(Vector3, Vector3);
float length(Vector3);

Vector3 cross(Vector3, Vector3);
Vector3 normalise(Vector3);

/********************************/

/*			Vector 4			*/

/********************************/

struct Vector4
{
	union
	{
		float xyzw[4];
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		struct
		{
			float r;
			float g;
			float b;
			float a;
		};
		struct
		{
			Vector3 xyz;
		};
	};

	Vector4();
	Vector4(float, float, float, float);
	Vector4(Vector3, float);

	float& operator[](int index)
	{
		return xyzw[index];
	}
};

//Simple arithmetic vector operations
Vector4 operator+(Vector4, Vector4);
Vector4 operator-(Vector4, Vector4);
Vector4 operator*(float, Vector4);
Vector4 operator/(Vector4, float);

void operator+=(Vector4&, Vector4);
void operator-=(Vector4&, Vector4);
void operator*=(Vector4&, float);
void operator/=(Vector4&, float);

Vector4 operator-(Vector4); //Unary

bool operator==(Vector4, Vector4);

//Common vector operations
float dot(Vector4, Vector4); //Does a 3d dot product using 4d vectors
float dot_4(Vector4, Vector4); //Does a 4d dot product
float length(Vector4);

Vector4 cross(Vector4, Vector4);

Vector4 normalise(Vector4);

/********************************/

/*			Matrix4x4			*/

/********************************/

struct Matrix4x4
{
	Vector4 columns[4];

	//Note: These functions are strictly read functions. To write, use [].
	Vector4 row(int i); //Returns the i'th row of the matrix
	Vector4 column(int i); //Returns the i'th column of the matrix

	Vector4& operator[](int index)
	{
		return columns[index];
	}
};

Vector4 operator*(Matrix4x4, Vector4);

Matrix4x4 operator*(Matrix4x4, Matrix4x4);
Matrix4x4 operator*(float, Matrix4x4);

Matrix4x4 identity();
Matrix4x4 transpose(Matrix4x4);

void scale(Matrix4x4&, float factor);
void rotate(Matrix4x4&, Vector4 axis, float angle);
void translate(Matrix4x4&, Vector4 translation);
void rotate(Matrix4x4&, Vector3 axis, float angle);
void translate(Matrix4x4&, Vector3 translation);

Matrix4x4 perspective(float fov, float aspect_ratio, float n, float f);
Matrix4x4 look_at(Vector4 eye_position, Vector4 target_position);
Matrix4x4 look_at(Vector4 eye_position, Vector4 forward_vector, Vector4 right_vector, Vector4 upward_vector);
Matrix4x4 look_at(Vector3 eye_position, Vector3 target_position);
Matrix4x4 look_at(Vector3 eye_position, Vector3 forward_vector, Vector3 right_vector, Vector3 upward_vector);

/********************************/

/*			Matrix3x3			*/

/********************************/

struct Matrix3x3
{
	Vector3 columns[3];

	//Note: These functions are strictly for reading matrix only.
	//		Use [] to write to matrix.
	Vector3 row(int i); //Returns the i'th row of the matrix
	Vector3 column(int i); //Returns the i'th column of the matrix

	Vector3& operator[](int index)
	{
		return columns[index];
	}
};

Matrix3x3 operator-(Matrix3x3);

Matrix3x3 operator+(Matrix3x3, Matrix3x3);
Matrix3x3 operator-(Matrix3x3, Matrix3x3);
Matrix3x3 operator*(Matrix3x3, Matrix3x3);
Matrix3x3 operator*(float, Matrix3x3);
Vector3 operator*(Matrix3x3, Vector3);
Matrix3x3 operator/(Matrix3x3, float);

void operator+=(Matrix3x3&, Matrix3x3);
void operator-=(Matrix3x3&, Matrix3x3);
void operator*=(Matrix3x3&, Matrix3x3);
void operator*=(Matrix3x3&, float);
void operator*=(Vector3&, Matrix3x3);
void operator/=(Matrix3x3&, float);

Matrix3x3 transpose(Matrix3x3);

/********************************/

/*			Quaternion			*/

/********************************/

//NOTE: Of the Quaternion code, I've only included thus far the minimum code needed to
//		implement the arcball.
struct Quaternion
{
	union
	{
		Vector4 xyzw;
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};
		struct
		{
			Vector3 xyz;
			float w;
		};
	};

	Quaternion();
	Quaternion(Vector3 axis, float angle);

	float& operator[](int index)
	{
		return xyzw[index];
	}
};

Quaternion operator*(Quaternion, Quaternion);
bool operator!=(Quaternion, Quaternion);
Quaternion operator/(Quaternion, float);

Quaternion conjugate(Quaternion);
Quaternion inverse(Quaternion);

float norm(Quaternion);

Quaternion compute_great_circle_point(Vector2);
Matrix3x3 quaternion_to_matrix(Quaternion);
Quaternion compute_rotation_between_quaternions(Quaternion, Quaternion);
Matrix3x3 compute_rotation_matrix_between_quaternions(Quaternion, Quaternion);