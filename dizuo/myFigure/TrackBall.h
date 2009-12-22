/*************************************************************/
/**					Filename: TRACKBALL.H		            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  14/05/2008	-	RYF Email:dizuo@126.com				**/
/**					-	Creation							**/	
/**					All Rights Reserved.					**/
/*************************************************************/
#ifndef TRACKBALL_H
#define TRACKBALL_H

#include <math.h>

#include "util/gtl/vec3.hpp"
#include "util/gtl/quat.hpp"

class Trackball 
{
private:  
	int width, height;  
	gtl::Quatf orientation;  
	gtl::Quatf startOrientation;  

	gtl::Vec3f startVector; 
	gtl::Vec3f endVector;  
	
	void mapToSphere(gtl::Vec3f &v);  
public: 
	Trackball(int w, int h)
		:width( w ), height( h ) 
	{}
	Trackball()
		:width(1024), height(768)
	{} 
	void SetWnd (int pw, int ph) 
	{
		width = pw; height = ph; 
	}
	void BeginTracking(int x, int y); 
	void Reset () 
	{
		orientation.makeIdentity();
	}
	void Tracking(int x, int y); 

	gtl::Matrix4f To4x4RotationMatrix(); 
	void toOpenGL(); 
}; 

#endif	/* TRACKBALL_H */