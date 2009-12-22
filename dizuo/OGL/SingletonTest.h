#include "Singleton.hpp"
#include <stdio.h>

class SingTest : public Singleton<SingTest>
{
	friend class Singleton<SingTest>;
public:
	void Test()
	{
		printf("This is the singleton...\n");
	}
private:
	SingTest(){};
	~SingTest(){};
};
