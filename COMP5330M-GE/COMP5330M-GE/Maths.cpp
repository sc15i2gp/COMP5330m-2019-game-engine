#include "Maths.h"

/****************************/

/*			Trig			*/

/****************************/

double cos_deg(double angle)
{
	return cos((angle / 180.0f)*PI);
}

double sin_deg(double angle)
{
	return sin((angle / 180.0f)*PI);
}

double tan_deg(double angle)
{
	return tan((angle / 180.0f)*PI);
}

/****************************/

/*			Vector4			*/

/****************************/

Vector4::Vector4()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
	this->w = 0.0f;
}


Vector4::Vector4(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector4::Vector4(Vector3 v, float d)
{
	this->xyz = v;
	this->w = d;
}

Vector4 operator+(Vector4 v, Vector4 w)
{
	v.x += w.x;
	v.y += w.y;
	v.z += w.z;
	return v;
}

Vector4 operator-(Vector4 v, Vector4 w)
{
	v.x -= w.x;
	v.y -= w.y;
	v.z -= w.z;
	return v;
}

Vector4 operator*(float d, Vector4 v)
{
	v.x *= d;
	v.y *= d;
	v.z *= d;
	return v;
}

Vector4 operator/(Vector4 v, float d)
{
	v.x /= d;
	v.y /= d;
	v.z /= d;
	return v;
}

void operator+=(Vector4& v, Vector4 w)
{
	v = v + w;
}

void operator-=(Vector4& v, Vector4 w)
{
	v = v - w;
}

void operator*=(Vector4& v, float d)
{
	v = d * v;
}

void operator/=(Vector4& v, float d)
{
	v = v / d;
}

Vector4 operator-(Vector4 v)
{
	return Vector4{ 0.0f, 0.0f, 0.0f, 1.0f } - v;
}

bool operator==(Vector4 v, Vector4 w)
{
	return v.x == w.x && v.y == w.y && v.z == w.z && v.w == w.w;
}

float dot(Vector4 v, Vector4 w)
{
	float d = 0.0f;
	d += v.x*w.x;
	d += v.y*w.y;
	d += v.z*w.z;
	return d;
}

float length(Vector4 v)
{
	return sqrt(dot(v,v));
}

Vector4 cross(Vector4 v, Vector4 w)
{
	Vector4 u(Vector3(), 1.0f);
	for (int i = 0; i < 3; ++i)
	{
		u[i] = v[(i + 1) % 3] * w[(i + 2) % 3] - v[(i + 2) % 3] * w[(i + 1) % 3];
	}
	return u;
}

Vector4 normalise(Vector4 v)
{
	return v / length(v);
}

/****************************/

/*			Vector3			*/

/****************************/

Vector3::Vector3()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->z = 0.0f;
}

Vector3::Vector3(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3::Vector3(Vector2 v, float d)
{
	this->xy = v;
	this->z = d;
}

Vector3 operator+(Vector3 v, Vector3 w)
{
	v.x += w.x;
	v.y += w.y;
	v.z += w.z;
	return v;
}

Vector3 operator-(Vector3 v, Vector3 w)
{
	v.x -= w.x;
	v.y -= w.y;
	v.z -= w.z;
	return v;
}

Vector3 operator*(float d, Vector3 v)
{
	v.x *= d;
	v.y *= d;
	v.z *= d;
	return v;
}

Vector3 operator/(Vector3 v, float d)
{
	v.x /= d;
	v.y /= d;
	v.z /= d;
	return v;
}

void operator+=(Vector3& v, Vector3 w)
{
	v = v + w;
}

void operator-=(Vector3& v, Vector3 w)
{
	v = w - w;
}

void operator*=(Vector3& v, float d)
{
	v = d * v;
}

void operator/=(Vector3& v, float d)
{
	v = v / d;
}

Vector3 operator-(Vector3 v)
{
	return Vector3{ 0.0f, 0.0f, 0.0f } - v;
}

//Common vector operations
float dot(Vector3 v, Vector3 w)
{
	float d = 0.0f;
	for (int i = 0; i < 3; ++i)
	{
		d += v[i] * w[i];
	}
	return d;
}

float length(Vector3 v)
{
	return sqrt(dot(v, v));
}

Vector3 cross(Vector3 v, Vector3 w)
{
	Vector3 u = {};
	for (int i = 0; i < 3; ++i)
	{
		u[i] = v[(i + 1) % 3] * w[(i + 2) % 3] - v[(i + 2) % 3] * w[(i + 1) % 3];
	}
	return u;
}

Vector3 normalise(Vector3 v)
{
	return v / length(v);
}

/****************************/

/*			Vector2			*/

/****************************/

float dot(Vector2 v, Vector2 w)
{
	float d = 0.0f;
	d += v.x * w.x;
	d += v.y * w.y;
	return d;
}

/********************************/

/*			Matrix4x4			*/

/********************************/

Vector4 Matrix4x4::row(int n)
{
	Vector4 v = {};
	for (int i = 0; i < 4; ++i)
	{
		v[i] = (*this)[i][n];
	}
	return v;
}

Vector4 Matrix4x4::column(int n)
{
	return (*this)[n];
}

Vector4 operator*(Matrix4x4 m, Vector4 v)
{
	Vector4 w = {};
	for (int i = 0; i < 4; ++i)
	{
		w[i] = dot(m.row(i), v);
	}
	return w;
}

Matrix4x4 operator*(Matrix4x4 m, Matrix4x4 n)
{
	Matrix4x4 p = {};
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			p[i][j] = dot(m.row(j), n.column(i));
		}
	}
	return p;
}

Matrix4x4 operator*(float d, Matrix4x4 m)
{
	for (int i = 0; i < 4; ++i)
	{
		m[i] *= d;
	}
	return m;
}

//m *= n equivalent to m = n * m
void operator*=(Matrix4x4& m, Matrix4x4 n)
{
	m = n * m;
}

void operator*=(Matrix4x4& m, float d)
{
	m = d * m;
}

Matrix4x4 identity()
{
	Matrix4x4 id = {};
	for (int i = 0; i < 4; ++i)
	{
		id[i][i] = 1.0f;
	}
	return id;
}

/*NOTE: This is an in place transpose. Not used since I introduced .row and .column
//		functions. If we need an in place tranpose later, come back here.
Matrix4x4 transpose(Matrix4x4 m)
{
	for (int i = 0; i < 3; ++i)
	{
		for (int j = i + 1; j < 4; ++j)
		{
			float temp = m[i][j];
			m[i][j] = m[j][i];
			m[j][i] = temp;
		}
	}
	return m;
}
*/

Matrix4x4 transpose(Matrix4x4 m)
{
	Matrix4x4 t = {};
	for (int i = 0; i < 4; ++i)
	{
		t[i] = m.row(i); //i'th column of t = i'th row of m
	}
}

//Applies scale of factor f to matrix m
void scale(Matrix4x4& m, float f)
{
	m *= f * identity();
}

//Applies a rotation about axis by angle to matrix m
void rotate(Matrix4x4& m, Vector4 axis, float angle)
{
	Vector3 axis_3 = axis.xyz;
	rotate(m, axis_3, angle);
}

//Applies translation v to matrix m
void translate(Matrix4x4& m, Vector4 v)
{
	m[3] += v;
}

void rotate(Matrix4x4& m, Vector3 axis, float angle)
{
	Quaternion q(axis, angle);
	m *= quaternion_to_matrix(q);
}

void translate(Matrix4x4& m, Vector3 v)
{
	Vector4 v_4(v, 1.0f);
	translate(m, v_4);
}

Matrix4x4 perspective(float fov, float aspect_ratio, float n, float f)
{
	Matrix4x4 p = {};
	if (fov > 0.0f && fov < 180.0f && n < f)
	{
		float cot_fov = 1.0f / tan_deg(fov / 2.0f);

		p[0][0] = cot_fov;
		p[1][1] = cot_fov * aspect_ratio;
		p[2][2] = (f + n) / (n - f);
		p[3][2] = (2.0f*f*n) / (n - f);
		p[2][3] = -1.0f;
	}
	p[3][3] = 1.0f;
	return p;
}

void print_vec(Vector4 v)
{
	OutputDebugStringf("%f %f %f %f\n", v.x, v.y, v.z, v.w);
}

Matrix4x4 look_at(Vector4 eye_position, Vector4 target_position)
{
	//Constant used to compute look_at's right vector
	Vector4 constant = { 0.0f, 1.0f, 0.0f, 1.0f };

	Vector4 target_dir = normalise(eye_position - target_position);
	//Constant may need to be changed since the right vector needs to be computed
	//using the cross product
	if (target_dir == constant || target_dir == -constant)
	{//If the cross product of target_dir and constant == 0, change constant vector
		constant = Vector4(1.0f, 0.0f, 0.0f, 1.0f);
	}

	Vector4 right_dir = normalise(cross(constant, target_dir));
	Vector4 up_dir = normalise(cross(target_dir, right_dir));

	print_vec(target_dir);
	print_vec(right_dir);
	print_vec(up_dir);

	Vector4 column_0(right_dir.x, up_dir.x, target_dir.x, 0.0f);
	Vector4 column_1(right_dir.y, up_dir.y, target_dir.y, 0.0f);
	Vector4 column_2(right_dir.z, up_dir.z, target_dir.z, 0.0f);
	Vector4 column_3(dot(right_dir, eye_position), dot(up_dir, eye_position),
		dot(target_dir, eye_position), 1.0f);

	Matrix4x4 camera;
	camera[0] = column_0;
	camera[1] = column_1;
	camera[2] = column_2;
	camera[3] = -column_3;
	return camera;
}

Matrix4x4 look_at(Vector3 eye_position, Vector3 target_position)
{
	return look_at(Vector4(eye_position, 1.0f), Vector4(target_position, 1.0f));
}

/********************************/

/*			Matrix3x3			*/

/********************************/

Vector3 Matrix3x3::row(int n)
{
	Vector3 v = {};
	for (int i = 0; i < 3; ++i)
	{
		v[i] = (*this)[i][n];
	}
	return v;
}

Vector3 Matrix3x3::column(int n)
{
	return (*this)[n];
}

Matrix3x3 operator-(Matrix3x3 m)
{
	return Matrix3x3{} - m;
}

Matrix3x3 operator+(Matrix3x3 m, Matrix3x3 n)
{
	for (int i = 0; i < 3; ++i)
	{
		m[i] += n[i]; //i'th column of m + i'th column of n
	}
	return m;
}

Matrix3x3 operator-(Matrix3x3 m, Matrix3x3 n)
{
	for (int i = 0; i < 3; ++i)
	{
		m[i] -= n[i]; //i'th column of m - i'th column of n
	}
	return m;
}

Matrix3x3 operator*(Matrix3x3 m, Matrix3x3 n)
{
	Matrix3x3 p = {};
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			p[i][j] = dot(m.row(j), n.column(i));
		}
	}
	return p;
}

Matrix3x3 operator*(float d, Matrix3x3 m)
{
	for (int i = 0; i < 3; ++i)
	{
		m[i] *= d;
	}
	return m;
}

Vector3 operator*(Matrix3x3 m, Vector3 v)
{
	Vector3 w = {};
	for (int i = 0; i < 3; ++i)
	{
		w[i] = dot(m.row(i), v);
	}
	return w;
}

Matrix3x3 operator/(Matrix3x3 m, float d)
{
	for (int i = 0; i < 3; ++i)
	{
		m[i] /= d;
	}
	return m;
}

void operator+=(Matrix3x3& m, Matrix3x3 n)
{
	m = m + n;
}

void operator-=(Matrix3x3& m, Matrix3x3 n)
{
	m = m - n;
}

void operator*=(Matrix3x3& m, Matrix3x3 n)
{
	m = n * m;
}

void operator*=(Matrix3x3& m, float d)
{
	m = d * m;
}

void operator*=(Vector3& v, Matrix3x3 m)
{
	v = m * v;
}

void operator/=(Matrix3x3& m, float d)
{
	m = m / d;
}

Matrix3x3 transpose(Matrix3x3 m)
{
	Matrix3x3 t = {};
	for (int i = 0; i < 3; ++i)
	{
		t[i] = m.row(i);
	}
	return t;
}

/********************************/

/*			Quaternion			*/

/********************************/

Quaternion::Quaternion()
{
	this->xyzw = {};
}

Quaternion::Quaternion(Vector3 axis, float angle)
{
	this->xyz = axis;
	this->w = angle;
}

Quaternion operator*(Quaternion q, Quaternion r)
{
	Quaternion s = {};
	s.w = q.w*r.w - dot(q.xyz, r.xyz);
	s.xyz = cross(q.xyz, r.xyz) + q.w*r.xyz + r.w*q.xyz;
	return s;
}

bool operator!=(Quaternion q, Quaternion r)
{
	return q.xyzw == r.xyzw;
}

Quaternion operator/(Quaternion q, float d)
{
	q.xyzw /= d;
	return q;
}

Quaternion conjugate(Quaternion q)
{
	q.xyz = -q.xyz;
}

Quaternion inverse(Quaternion q)
{
	return conjugate(q) / norm(q);
}

float norm(Quaternion q)
{
	return dot(q.xyzw, q.xyzw);
}

Quaternion compute_great_circle_point(Vector2 v)
{
	Quaternion q = {};

	float z = sqrt(1.0f - dot(v, v));
	q.xyz = Vector3(v, z);
	q.w = 0.0f;

	return q;
}

Matrix4x4 quaternion_to_matrix(Quaternion q)
{
	Matrix4x4 m = {};
	m[0][0] = 1.0f - 2.0f*(q.y*q.y + q.z*q.z);
	m[0][1] = 2.0f*(q.x*q.y + q.w*q.z);
	m[0][2] = 2.0f*(q.x*q.z - q.w*q.y);
	m[1][0] = 2.0f*(q.x*q.y - q.w*q.z);
	m[1][1] = 1.0f - 2.0f*(q.x*q.x + q.z*q.z);
	m[1][2] = 2.0f*(q.y*q.z + q.w*q.x);
	m[2][0] = 2.0f*(q.x*q.z + q.w*q.y);
	m[2][1] = 2.0f*(q.y*q.z - q.w*q.x);
	m[2][2] = 1.0f - 2.0f*(q.x*q.x + q.y*q.y);
	m[3][3] = 1.0f;
	return m;
}

Matrix4x4 compute_rotation_between_quaternions(Quaternion q, Quaternion r)
{
	Quaternion s = r * inverse(q);
	return quaternion_to_matrix(s);
}