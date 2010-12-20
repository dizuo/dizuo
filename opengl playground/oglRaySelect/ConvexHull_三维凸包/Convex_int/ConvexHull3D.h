#ifndef CONVEX_HULL_3D
#define CONVEX_HULL_3D

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

namespace ConvexHull
{

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
/*Define Boolean type */
enum {	FALSE, TRUE };
/* Define vertex indices. */
#define X   0
#define Y   1
#define Z   2

/* Define flags */
#define ONHULL   	TRUE
#define REMOVED  	TRUE
#define VISIBLE  	TRUE
#define PROCESSED	TRUE
#define SAFE		1000000		/* Range of safe coord values. */

/* Define structures for vertices, edges and faces */
typedef struct tVertexStructure tsVertex;
typedef tsVertex *tVertex;	//pointer

typedef struct tEdgeStructure tsEdge;
typedef tsEdge	*tEdge;		//pointer

typedef struct tFaceStructure tsFace;
typedef tsFace	*tFace;		//pointer

struct tVertexStructure {
	int 	v[3];
	int		vnum;
	tEdge   duplicate;	        /* pointer to incident cone edge (or NULL) */
	bool    onhull;		/* T iff point on hull. */
	bool	mark;		/* T iff point already processed. */
	tVertex  next, prev;
};

struct tEdgeStructure {
	tFace    adjface[2];
	tVertex  endpts[2];
	tFace    newface;            /* pointer to incident cone face. */
	bool     is_delete;		/* T iff edge should be delete. */
	tEdge    next, prev;
};

struct tFaceStructure {
	tEdge    edge[3];
	tVertex  vertex[3];
	bool	    visible;	        /* T iff face visible from new point. */
	tFace    next, prev;
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class ConvexHull3D
{
public:
	ConvexHull3D()
		: vertices(0)
		, edges(0)
		, faces(0)
		, debug(FALSE)
		, check(FALSE)
	{}

	/*memeber variable definitions */
	tVertex vertices;
	tEdge edges;
	tFace faces;
	bool debug;
	bool check;

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	/* Function declarations */
	tVertex MakeNullVertex( void );
	void    ReadVertices( const char* fileName );
	void    Print( void );
	void    SubVec( int a[3], int b[3], int c[3]);
	void    DoubleTriangle( void );
	void    ConstructHull( void );
	bool	AddOne( tVertex p );
	int     VolumeSign(tFace f, tVertex p);
	int 	Volumei( tFace f, tVertex p );
	tFace	MakeConeFace( tEdge e, tVertex p );
	void    MakeCcw( tFace f, tEdge e, tVertex p );
	tEdge   MakeNullEdge( void );
	tFace   MakeNullFace( void );
	tFace   MakeFace( tVertex v0, tVertex v1, tVertex v2, tFace f );
	void    CleanUp( tVertex *pvnext );
	void    CleanEdges( void );
	void    CleanFaces( void );
	void    CleanVertices( tVertex *pvnext );
	bool	Collinear( tVertex a, tVertex b, tVertex c );
	void    CheckEuler(int V, int E, int F );
	void	PrintPoint( tVertex p );
	void    Checks( void );
	void	Consistency( void );
	void	Convexity( void );
	void	PrintOut( tVertex v );
	void	PrintVertices( void );
	void	PrintEdges( void );
	void	PrintFaces( void );
	void	CheckEndpts ( void );
	void	EdgeOrderOnFaces ( void );
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
};

} //namespace ConvexHull
#endif