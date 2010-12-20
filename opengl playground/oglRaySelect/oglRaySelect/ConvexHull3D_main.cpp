#include <iostream>
#include "ConvexHull3D.h"

using namespace std;

int main()
{
	using namespace ConvexHull;
	ConvexHull3D convexHullTest;

	//F==0
	convexHullTest.ReadVertices("pyramid.txt");
	//convexHullTest.ReadVertices("sphere.txt");
	convexHullTest.DoubleTriangle();
	convexHullTest.ConstructHull();
	convexHullTest.EdgeOrderOnFaces();
	convexHullTest.Print();

	system("PAUSE");

	return 0;
}

//bug: AddOne中adjFace[1]为NULL，遍历的时候非法操作->

/*
%% Vertices:    V = 9
%% index:       x       y       z
%%     4:       0       0       0
%%     7:       1874    -1783   4000
%%     8:       1874    1783    4000
%%     9:       3763    -3579   0
%%    10:       3763    3579    0
%%     0:       -3763   -3579   0
%%     1:       -3763   3579    0
%%     2:       -1874   -1783   4000
%%     3:       -1874   1783    4000

%% Faces:       F = 14
%% List of all faces:
%%      v0      v1      v2      (vertex indices)
%%      2       1       0
%%      0       1       4
%%      2       0       7
%%      2       7       8
%%      0       4       9
%%      7       0       9
%%      8       7       9
%%      4       1       10
%%      1       8       10
%%      9       4       10
%%      8       9       10
%%      1       2       3
%%      2       8       3
%%      8       1       3

%% Edges:       E = 21
*/