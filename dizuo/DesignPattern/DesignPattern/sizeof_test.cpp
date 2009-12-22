#include <iostream>
using namespace std;

template<typename T>
int getLen(T& arr)    
{    
	int n;  
	n=sizeof(arr) / sizeof(*arr); 
	return n;  
}

int main()
{
	char buffer[] ={"the test of sizeof keyword"};
	for (size_t bg=0; bg<getLen(buffer); bg++)
	{
		cout << buffer[bg];
	}
	cout << endl;

	return 0;
}