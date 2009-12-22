#include "TrackBall.h"
//#include <stdio.h>
#include "util/gtl/matrix4.hpp"
#include <GL/glut.h>

#define  INVSQRT2 1.0f/sqrt(2.0)  


void Trackball::mapToSphere(gtl::Vec3f &v)
{        
	float len2;         
	v.x() = ( 2 * v.x() ) / (float)( width - 1 )-1;        
	v.y() = 1 - ( 2 * v.y() ) / (float)( height - 1 );        
	v.z() = 0;        
	
	len2 = v.length();
	if( len2<INVSQRT2 )  
		v.z() = sqrt(1.0f-len2); // We are on the sphere        
	else                
		v.z() = 1.0f/(2*sqrt(len2)); // On the hyperbole 
}  

void Trackball::Tracking(int x, int y)
{        
	gtl::Quatf q;    
	gtl::Vec3f endVector( (float)x, (float)y, 0 );       

	mapToSphere(endVector);        
	
	q.setValue(startVector, endVector); 
	orientation = q * startOrientation; 
	orientation.normalize();
} 

void Trackball::BeginTracking(int x, int y) 
{     
	startVector = gtl::Vec3f( (float)x, (float)y, 0); 
	startOrientation = orientation;        
	mapToSphere(startVector); 
} 

gtl::Matrix4f Trackball::To4x4RotationMatrix()
{        
	return orientation.getMatrix();
}  

void Trackball::toOpenGL()
{
	gtl::Matrix4f mat = orientation.getMatrix();
	glMultMatrixf( *mat );
} 