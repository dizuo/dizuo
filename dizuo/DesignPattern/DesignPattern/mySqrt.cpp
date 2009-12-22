#include <iostream>
#include <cmath>
using namespace std;

inline float qyRSqrt( float x ) 
{
	// NOTE: allows x == 0
	long i;
	float y, r;

	y = x * 0.5f;
	i = *reinterpret_cast<long *>( &x );
	i = 0x5f3759df - ( i >> 1 );
	r = *reinterpret_cast<float *>( &i );
	r = r * ( 1.5f - r * r * y );
	return r;
}
inline float quickSqrt ( float x )
{
	union   {
		int ipart ;
		float fpart ;
	} con1 ;

	union   {
		int ipart ;
		float fpart ;
	} con2 ;

	con1.fpart = x ;
	con2.fpart = x ;

	con1.ipart = 0x1FBCF800 + ( con1.ipart >> 1 ) ;
	con2.ipart = 0x5f3759df - ( con2.ipart >> 1 ) ;

	return 0.5f * ( con1.fpart + x * con2.fpart ) ;
}

int main()
{
	float testCase[10] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
	for (size_t i(0); i<10; i++)
	{
		cout << sqrt(testCase[i]) << "\t";
		cout << qyRSqrt(testCase[i]) << "\t";
		cout << quickSqrt(testCase[i]) << endl;
	}

	return 0;
}