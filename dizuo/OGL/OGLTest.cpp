/* 
** ================================================================== 
** 全部的测试主程序
** ================================================================== 
*/ 
#include <iostream>

#include "gtl/quat.hpp"
//#include "TrackBallTest.h"
//#include "matrix4Test.h"
// #include "cameraTest.h"
// #include "TextureTest.h"
#include "SingletonTest.h"
#include "Config.h"

int main()
{
	using namespace std;
	using namespace gtl;

/*	关于库中的疑点：
 	//! Return pointer to the matrix' 4x4 array.
	Type* operator* ()
	{ 
		return &(m_data[0][0]); 
	}
	/*      operator Type * ()
	{ 
	return &(m_data[0][0]); 
	}*/

/*
	gtl::Matrix4i mat;
	int* m = *mat;
	cout << *m <<endl;
	cout << *(m+5) <<endl;
	cout << *(m+10) <<endl;
	cout << *(m+15) <<endl;
	
	cout << "-----------------------------------------------" << endl;	
	gtl::Quatf quat(1.0, 2.0, 3.0, 4.0);
	cout << " x = " << quat.x() << " ";
	cout << " y = " << quat.y() << " ";
	cout << " z = " << quat.z() << " ";
	cout << " w = " << quat.w() << endl;
	cout << "-----------------------------------------------" << endl;	
	for (size_t i(0); i<4; i++)
		cout << "quat" << "[" << i << "] = " << quat[i] << " ";
	cout << endl;
	
	// quat[4];		// 异常测试

	gtl::Quatf congQuat = quat.conjugate();
	cout << "-----------------------------------------------" << endl;	
	cout << " x = " << congQuat.x() << " ";
	cout << " y = " << congQuat.y() << " ";
	cout << " z = " << congQuat.z() << " ";
	cout << " w = " << congQuat.w() << endl;
	cout << "-----------------------------------------------" << endl;	

	_LOG("Draw the ColorBox at [" + toStr<int>(0) + " "
							      + toStr<int>(0) + " "
								  + toStr<int>(0) + "]" );

	float vfOffset[3], vfRoffset[3];
	Config cfg("ConfigTest.txt");
	cfg.loadBlock("point_light");
	cfg.getFloat("offset", vfOffset, 3);
	cfg.getFloat("render_offset", vfRoffset, 3);
	for(size_t i(0); i<3; i++) {
		cout << vfOffset[i] <<" ";
	} cout << endl;
	for(size_t i(0); i<3; i++) {
		cout << vfRoffset[i] <<" ";
	} cout << endl;
*/

//   	TrackBallTest test;
//   	test.Excute();

//  	Matrix4Test Test;
//  	Test.Excute();
	
//  	CameraTest test;
//  	test.Excute();

// 	TextureTest test;
// 	test.Excute();
	SingTest::GetInstance()->Test();
	SingTest::Kill();

	return 0;
}