// Timothy Chan "ch3d.cc" 12/02 3-d lower hull (in C++)

// a simple implementation of the O(n log n) divide-and-conquer algorithm

// input: coordinates of points
// n x_0 y_0 z_0 ... x_{n-1} y_{n-1} z_{n-1}

// output: indices of facets
// i_1 j_1 k_1 i_2 j_2 k_2 ...
// warning: ignores degeneracies and robustness
// space: uses 6n pointers

#include <iostream>
#include <fstream>

using namespace std;


struct Point {
	double x, y, z;
	Point *prev, *next;
	void act() {
		if (prev->next != this) prev->next = next->prev = this; // insert
		else { prev->next = next; next->prev = prev; } // delete
	}
};
	
const double INF = 1e99;
static Point nil = {INF, INF, INF, 0, 0};
Point *NIL = &nil;

inline double turn(Point *p, Point *q, Point *r) 
{
	// <0 iff cw
	if (p == NIL || q == NIL || r == NIL) return 1.0;
	return (q->x-p->x)*(r->y-p->y) - (r->x-p->x)*(q->y-p->y);
}

inline double time(Point *p, Point *q, Point *r) 
{
	// when turn changes
	if (p == NIL || q == NIL || r == NIL) return INF;
	return ((q->x-p->x)*(r->z-p->z) - (r->x-p->x)*(q->z-p->z)) / turn(p,q,r);
}

Point *sort(Point P[], int n) 
{
	// mergesort	
	Point *a, *b, *c, head;
	
	if (n == 1) { P[0].next = NIL; return P; }
	
	a = sort(P, n/2);
	b = sort(P+n/2, n-n/2);
	c = &head;
	
	do {
		if (a->x < b->x) { c = c->next = a; a = a->next; }
		else { c = c->next = b; b = b->next; }
	}while (c != NIL);

	return head.next;
}

void hull(Point *list, int n, Point **A, Point **B) 
{
	// the algorithm
	Point *u, *v, *mid;
	double t[6], oldt, newt;
	int i, j, k, l, minl;

	if (n == 1) 
	{
		A[0] = list->prev = list->next = NIL; 
		return; 
	}
	
	for (u = list, i = 0; i < n/2-1; u = u->next, i++) ;
	
	mid = v = u->next;
	hull(list, n/2, B, A); // recurse on left and right sides
	hull(mid, n-n/2, B+n/2*2, A+n/2*2);
	
	for ( ; ; ) // find initial bridge
	{
		if (turn(u, v, v->next) < 0) 
			v = v->next;
		else if (turn(u->prev, u, v) < 0) 
			u = u->prev;
		else 
			break;
		
		// merge by tracking bridge uv over time
		for (i = k = 0, j = n/2*2, oldt = -INF; ; oldt = newt) 
		{
			t[0] = time(B[i]->prev, B[i], B[i]->next);
			t[1] = time(B[j]->prev, B[j], B[j]->next);
			t[2] = time(u, u->next, v);
			t[3] = time(u->prev, u, v);
			t[4] = time(u, v->prev, v);
			t[5] = time(u, v, v->next);
			
			for (newt = INF, l = 0; l < 6; l++)
				if (t[l] > oldt && t[l] < newt) 
				{
					minl = l; 
					newt = t[l]; 
				}
			if (newt == INF)
				break;
			
			switch (minl) 
			{
				case 0: 
					if (B[i]->x < u->x) 
						A[k++] = B[i]; 
					B[i++]->act(); 
					break;
				case 1: 
					if (B[j]->x > v->x) 
						A[k++] = B[j]; 
					B[j++]->act(); 
					break;
				case 2: 
					A[k++] = u = u->next; 
					break;
				case 3:
					A[k++] = u; 
					u = u->prev; 
					break;
				case 4: 
					A[k++] = v = v->prev; 
					break;
				case 5: 
					A[k++] = v; 
					v = v->next; 
					break;
			}
		}

		A[k] = NIL;

		u->next = v; v->prev = u; // now go back in time to update pointers
		
		for (k--; k >= 0; k--)
		{
			if (A[k]->x <= u->x || A[k]->x >= v->x) 
			{
				A[k]->act();
				if (A[k] == u) 
					u = u->prev; 
				else if (A[k] == v) 
					v = v->next;
			}
			else 
			{
				u->next = A[k]; A[k]->prev = u; v->prev = A[k]; A[k]->next = v;
				if (A[k]->x < mid->x) 
					u = A[k]; 
				else 
					v = A[k];
			}
		}//for

	}//for(; ;)
}

// For simplicity:  
// we assume that the points are non-degenerate: 
//	no four points lie on a common plane.
// 	no three points lie on a common vertical plane.

int main() 
{
	int n, i;

// 	cin >> n;
// 	Point *P = new Point[n]; // input
// 	for (i = 0; i < n; i++) { cin >> P[i].x; cin >> P[i].y; cin >> P[i].z; }
	ifstream inFile("input_point.txt");

	inFile >> n;
	Point *P = new Point[n]; // input
	for (int i=0; i<n; i++)
	{
		inFile >> P[i].x; inFile >> P[i].y; inFile >> P[i].z;
	} inFile.close();
	
	Point *list = sort(P, n);
	Point **A = new Point *[2*n], **B = new Point *[2*n];
	
	hull(list, n, A, B);
	
	for (i = 0; A[i] != NIL; A[i++]->act()) // output
		cout << A[i]->prev-P << " " << A[i]-P << " " << A[i]->next-P << "\n";
	
	delete A; delete B; delete P;

	system("PAUSE");

	return 0;
}