// Maths.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
	return 0;
}
