// Maths.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
Group Project
Game Engine Maths Library
Maths.cpp
*/

#include <iostream>
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"

using namespace std;

int main()
{
	Vector3 vector1(2.0, 4.0, 5.0);
	Vector3 vector2(8.0, -2.0, -3.0);
	cout << vector1.getX() << " " << vector1.getY() << " " << vector1.getZ() << endl;
	vector1.setX(5.0);
	cout << vector1.length() << endl;
	cout << vector1.normalize().getX() << " " << vector1.normalize().getY() << " " << vector1.normalize().getZ() << endl;
	cout << vector1.normalize().length() << endl;
	Vector3 sum(vector1 + vector2);
	cout << sum.getX() << " " << sum.getY() << " " << sum.getZ() << endl;
	Vector3 mult(vector1 * 2.0);
	cout << mult.getX() << " " << mult.getY() << " " << mult.getZ() << endl;
	cout << vector1 * vector2 << endl;
	Vector3 cross(vector1.cross(vector2));
	cout << cross.getX() << " " << cross.getY() << " " << cross.getZ() << endl;

	cout << endl << endl;

	Vector4 vector3(vector2, 1.0);
	cout << vector3.getX() << " " << vector3.getY() << " " << vector3.getZ() << " " << vector3.getW() << endl;
	cout << vector3.normalize().length() << endl;
	Vector4 vector4(0.5, 0.6, -0.2, 1.0);
	Vector4 sum2(vector3 + vector4);
	cout << sum2.getX() << " " << sum2.getY() << " " << sum2.getZ() << " " << sum2.getW() << endl;
	cout << vector3 * vector4 << endl;

	cout << endl << endl;

	Matrix3x3 matrix3(1.0, 4.0, -3.0,
		5.0, -2.0, 1.0,
		1.0, 2.0, 3.0);
	cout << matrix3.getaa() << " " << matrix3.getab() << " " << matrix3.getac() << endl
		<< matrix3.getba() << " " << matrix3.getbb() << " " << matrix3.getbc() << endl
		<< matrix3.getca() << " " << matrix3.getcb() << " " << matrix3.getcc() << endl << endl;
	Matrix3x3 tra3(matrix3.transpose());
	cout << tra3.getaa() << " " << tra3.getab() << " " << tra3.getac() << endl
		<< tra3.getba() << " " << tra3.getbb() << " " << tra3.getbc() << endl
		<< tra3.getca() << " " << tra3.getcb() << " " << tra3.getcc() << endl << endl;
	Matrix3x3 matrix4(2.0, -5.0, -1.0,
		-2.0, 2.0, -2.0,
		1.0, 4.0, -3.0);
	matrix4.setaa(3.0);
	Matrix3x3 mult3(matrix3 * matrix4);
	cout << mult3.getaa() << " " << mult3.getab() << " " << mult3.getac() << endl
		<< mult3.getba() << " " << mult3.getbb() << " " << mult3.getbc() << endl
		<< mult3.getca() << " " << mult3.getcb() << " " << mult3.getcc() << endl << endl;
	Vector3 mv3(matrix4 * vector1);
	cout << mv3.getX() << " " << mv3.getY() << " " << mv3.getZ() << endl;

	cout << endl << endl;

	Matrix4x4 matrix1(1.0, 3.0, 2.0, 5.0,
		2.0, 4.0, 5.0, 2.0,
		3.0, 3.0, 4.0, 4.0,
		4.0, 1.0, 2.0, 1.0);
	Matrix4x4 matrix2(-2.0, 3.0, -1.0, 3.0,
		-2.0, -3.0, 1.0, 2.0,
		3.0, 1.0, -4.0, -2.0,
		-4.0, 3.0, 2.0, -1.0);
	cout << matrix1.getaa() << " " << matrix1.getab() << " " << matrix1.getac() << " " << matrix1.getad() << endl
		<< matrix1.getba() << " " << matrix1.getbb() << " " << matrix1.getbc() << " " << matrix1.getbd() << endl
		<< matrix1.getca() << " " << matrix1.getcb() << " " << matrix1.getcc() << " " << matrix1.getcd() << endl
		<< matrix1.getda() << " " << matrix1.getdb() << " " << matrix1.getdc() << " " << matrix1.getdd() << endl << endl;
	Matrix4x4 msum(matrix1 + matrix2);
	cout << msum.getaa() << " " << msum.getab() << " " << msum.getac() << " " << msum.getad() << endl
		<< msum.getba() << " " << msum.getbb() << " " << msum.getbc() << " " << msum.getbd() << endl
		<< msum.getca() << " " << msum.getcb() << " " << msum.getcc() << " " << msum.getcd() << endl
		<< msum.getda() << " " << msum.getdb() << " " << msum.getdc() << " " << msum.getdd() << endl << endl;
	Matrix4x4 neg(matrix1.negative());
	Matrix4x4 tra(matrix1.transpose());
	cout << neg.getaa() << " " << neg.getab() << " " << neg.getac() << " " << neg.getad() << endl
		<< neg.getba() << " " << neg.getbb() << " " << neg.getbc() << " " << neg.getbd() << endl
		<< neg.getca() << " " << neg.getcb() << " " << neg.getcc() << " " << neg.getcd() << endl
		<< neg.getda() << " " << neg.getdb() << " " << neg.getdc() << " " << neg.getdd() << endl << endl;
	cout << tra.getaa() << " " << tra.getab() << " " << tra.getac() << " " << tra.getad() << endl
		<< tra.getba() << " " << tra.getbb() << " " << tra.getbc() << " " << tra.getbd() << endl
		<< tra.getca() << " " << tra.getcb() << " " << tra.getcc() << " " << tra.getcd() << endl
		<< tra.getda() << " " << tra.getdb() << " " << tra.getdc() << " " << tra.getdd() << endl << endl;
	Matrix4x4 sca(matrix2 * 3);
	cout << sca.getaa() << " " << sca.getab() << " " << sca.getac() << " " << sca.getad() << endl
		<< sca.getba() << " " << sca.getbb() << " " << sca.getbc() << " " << sca.getbd() << endl
		<< sca.getca() << " " << sca.getcb() << " " << sca.getcc() << " " << sca.getcd() << endl
		<< sca.getda() << " " << sca.getdb() << " " << sca.getdc() << " " << sca.getdd() << endl << endl;
	Matrix4x4 mmult(matrix1 * matrix2);
	cout << mmult.getaa() << " " << mmult.getab() << " " << mmult.getac() << " " << mmult.getad() << endl
		<< mmult.getba() << " " << mmult.getbb() << " " << mmult.getbc() << " " << mmult.getbd() << endl
		<< mmult.getca() << " " << mmult.getcb() << " " << mmult.getcc() << " " << mmult.getcd() << endl
		<< mmult.getda() << " " << mmult.getdb() << " " << mmult.getdc() << " " << mmult.getdd() << endl << endl;
	Vector4 mv(matrix1 * vector4);
	cout << mv.getX() << " " << mv.getY() << " " << mv.getZ() << " " << mv.getW() << endl;
	return 0;
}
