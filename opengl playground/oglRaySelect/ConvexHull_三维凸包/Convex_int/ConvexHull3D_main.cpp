#include <iostream>
#include "ConvexHull3D.h"

using namespace std;

int main()
{
	using namespace ConvexHull;
	ConvexHull3D convexHullTest;

	convexHullTest.ReadVertices("sphere.txt");
	convexHullTest.DoubleTriangle();
	convexHullTest.ConstructHull();
	convexHullTest.EdgeOrderOnFaces();
	convexHullTest.Print();

	system("PAUSE");

	return 0;
}

//pyramid
/*
%% Vertices:    V = 9
%% index:       x       y       z
%%     4:       0       0       0
%%     5:       18      -17     40
%%     6:       18      17      40
%%     7:       37      -35     0
%%     8:       37      35      0
%%     0:       -37     -35     0
%%     1:       -37     35      0
%%     2:       -18     -17     40
%%     3:       -18     17      40

%% Faces:       F = 14

%% List of all faces:
%%      v0      v1      v2      (vertex indices)
%%      2       1       0
%%      0       1       4
%%      2       0       5
%%      2       5       6
%%      0       4       7
%%      5       0       7
%%      6       5       7
%%      4       1       8
%%      1       6       8
%%      7       4       8
%%      6       7       8
%%      1       2       3
%%      2       6       3
%%      6       1       3
*/