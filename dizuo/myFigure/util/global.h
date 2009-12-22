#include <Windows.h>
#include <Gl/Gl.H>
#include <Gl/Glu.H>
#include <Gl/GLaux.H>
#include <cmath>

#pragma comment (lib,"OpenGL32.Lib")
#pragma comment (lib,"GLu32.Lib")
#pragma comment (lib,"GLaux.Lib")
#pragma comment (lib,"WinMm.Lib")

#define SAFE_DELETE(p) {if(p) {delete p; p = NULL;}}
#define BUF_SIZE	512

#define PrintData(matrix,totalsize,row)	\
	printf("\n----------------------------------------\n");	\
	for (size_t i=0; i<totalsize; i++){		\
	printf(" %f", matrix[i]);			\
	if( (i+1)%row == 0 )				\
		printf("\n");					\
	else								\
		printf(" ");					\
	}									\
	printf("----------------------------------------\n");