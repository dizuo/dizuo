#include <iostream>
#include <string>
using namespace std;

#include "Figure.h"
#include "HTreeTest.h"
#include "ScriptParserTest.h"

int main()
{
	HTreeTestCase::run1();
	HTreeTestCase::run2();
	
	ScriptParserTestCase::run1();
	//ScriptParserTestCase::run2();

	system("PAUSE");
	return 0;
}
/*
The HNode Tree :
  one
    two
      apple
      banana
      peach
    three
      china
      england
The Joint Tree :
  one
    two
      apple
      banana
      peach
    three
      china
      england
*/
