// Maths.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
Group Project
Game Engine Maths Library
Maths.cpp
*/

#include <iostream>
#include "Vector.h"
#include "Matrix.h"

using namespace std;

int main()
{
	Vector vector1(2.0, 4.0, 5.0);
	Vector vector2(8.0, -2.0, -3.0);
	cout << vector1.getX() << " " << vector1.getY() << " " << vector1.getZ() << endl;
	vector1.setX(5.0);
	cout << vector1.length() << endl;
	cout << vector1.normalize().getX() << " " << vector1.normalize().getY() << " " << vector1.normalize().getZ() << endl;
	cout << vector1.normalize().length() << endl;
	Vector sum(vector1 + vector2);
	cout << sum.getX() << " " << sum.getY() << " " << sum.getZ() << endl;
	Vector mult(vector1 * 2.0);
	cout << mult.getX() << " " << mult.getY() << " " << mult.getZ() << endl;
	cout << vector1 * vector2 << endl;
	Vector cross(vector1.cross(vector2));
	cout << cross.getX() << " " << cross.getY() << " " << cross.getZ() << endl;

	cout << endl << endl;

	Matrix matrix1(1.0, 3.0, 2.0, 5.0,
		2.0, 4.0, 5.0, 2.0,
		3.0, 3.0, 4.0, 4.0,
		4.0, 1.0, 2.0, 1.0);
	Matrix matrix2(-2.0, 3.0, -1.0, 3.0,
		-2.0, -3.0, 1.0, 2.0,
		3.0, 1.0, -4.0, -2.0,
		-4.0, 3.0, 2.0, -1.0);
	cout << matrix1.getaa() << " " << matrix1.getab() << " " << matrix1.getac() << " " << matrix1.getad() << endl
		<< matrix1.getba() << " " << matrix1.getbb() << " " << matrix1.getbc() << " " << matrix1.getbd() << endl
		<< matrix1.getca() << " " << matrix1.getcb() << " " << matrix1.getcc() << " " << matrix1.getcd() << endl
		<< matrix1.getda() << " " << matrix1.getdb() << " " << matrix1.getdc() << " " << matrix1.getdd() << endl << endl;
	Matrix msum(matrix1 + matrix2);
	cout << msum.getaa() << " " << msum.getab() << " " << msum.getac() << " " << msum.getad() << endl
		<< msum.getba() << " " << msum.getbb() << " " << msum.getbc() << " " << msum.getbd() << endl
		<< msum.getca() << " " << msum.getcb() << " " << msum.getcc() << " " << msum.getcd() << endl
		<< msum.getda() << " " << msum.getdb() << " " << msum.getdc() << " " << msum.getdd() << endl << endl;
	Matrix neg(matrix1.negative());
	Matrix tra(matrix1.transpose());
	cout << neg.getaa() << " " << neg.getab() << " " << neg.getac() << " " << neg.getad() << endl
		<< neg.getba() << " " << neg.getbb() << " " << neg.getbc() << " " << neg.getbd() << endl
		<< neg.getca() << " " << neg.getcb() << " " << neg.getcc() << " " << neg.getcd() << endl
		<< neg.getda() << " " << neg.getdb() << " " << neg.getdc() << " " << neg.getdd() << endl << endl;
	cout << tra.getaa() << " " << tra.getab() << " " << tra.getac() << " " << tra.getad() << endl
		<< tra.getba() << " " << tra.getbb() << " " << tra.getbc() << " " << tra.getbd() << endl
		<< tra.getca() << " " << tra.getcb() << " " << tra.getcc() << " " << tra.getcd() << endl
		<< tra.getda() << " " << tra.getdb() << " " << tra.getdc() << " " << tra.getdd() << endl << endl;
	Matrix sca(matrix2 * 3);
	cout << sca.getaa() << " " << sca.getab() << " " << sca.getac() << " " << sca.getad() << endl
		<< sca.getba() << " " << sca.getbb() << " " << sca.getbc() << " " << sca.getbd() << endl
		<< sca.getca() << " " << sca.getcb() << " " << sca.getcc() << " " << sca.getcd() << endl
		<< sca.getda() << " " << sca.getdb() << " " << sca.getdc() << " " << sca.getdd() << endl << endl;
	Matrix mmult(matrix1 * matrix2);
	cout << mmult.getaa() << " " << mmult.getab() << " " << mmult.getac() << " " << mmult.getad() << endl
		<< mmult.getba() << " " << mmult.getbb() << " " << mmult.getbc() << " " << mmult.getbd() << endl
		<< mmult.getca() << " " << mmult.getcb() << " " << mmult.getcc() << " " << mmult.getcd() << endl
		<< mmult.getda() << " " << mmult.getdb() << " " << mmult.getdc() << " " << mmult.getdd() << endl << endl;
	return 0;
}
