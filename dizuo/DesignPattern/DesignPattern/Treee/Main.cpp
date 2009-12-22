#include <algorithm>
#include <string>
#include <iostream>

#include "Tree.hpp"

#include "UtilTest.hpp"
#include "ScriptParserTestCase.hpp"


using namespace kptree;
using namespace util;
using namespace std;

#pragma comment(lib, "tinyxml.lib")

// class Node
// {};
// 
// template< class T>
// class Parser
// {
// };
// 
// template<class ParserType = Parser<Node> >
// class Tree : ParserType
// {
// };
// 
// Tree<> x; 

int main(int, char **)
{
	//common util test:
	TreeUtilTestCase::run();
	//script util test:
	ScriptParserTestCase::run1();
	ScriptParserTestCase::run2();
	ScriptParserTestCase::run3();
	ScriptParserTestCase::run4();	

	system("PAUSE");
	return 0;
}

//只有前序，后序，没有中序.
/*
-----
one
  two
    apple
    banana
      cherry
    peach
  three
-----
*/

/*
1               0       0       0
2               5       -4      0
3               9       -8      0
4               5       -8      4
5               8       -12     4
6               5       -12     7
7               2       -12     4
8               5       -12     1
9               1       -8      6.03983e-007
10              5       -8      -4
11              -5      -4      7.54979e-007
12              -1      -8      7.54979e-007
13              2       -12     7.54979e-007
14              -1      -12     3
15              -4      -12     1.20797e-006
16              -1      -12     -3
17              -5      -8      4
18              -9      -8      1.35896e-006
19              -5      -8      -4
*/

/*
1               0       0       0
2               5       -4      0
3               9       -8      0
4               5       -8      4
5               8       -12     4
6               5       -12     7
7               2       -12     4
8               5       -12     1
9               1       -8      6.03983e-007
10              5       -8      -4
11              -5      -4      7.54979e-007
12              -1      -8      7.54979e-007
13              2       -12     7.54979e-007
14              -1      -12     3
15              -4      -12     1.20797e-006
16              -1      -12     -3
17              -5      -8      4
18              -9      -8      1.35896e-006
19              -5      -8      -4
*/

/*----------------------------------------------------------
                The Joint Type XMLScriptParser Test Case :
Type = ROOT     Name = 1
Type = JOINT    Name = 2
Type = END      Name = 3
Type = JOINT    Name = 4
Type = END      Name = 5
Type = END      Name = 6
Type = END      Name = 7
Type = END      Name = 8
Type = END      Name = 9
Type = END      Name = 10
Type = JOINT    Name = 11
Type = JOINT    Name = 12
Type = END      Name = 13
Type = END      Name = 14
Type = END      Name = 15
Type = END      Name = 16
Type = END      Name = 17
Type = END      Name = 18
Type = END      Name = 19
----------------------------------------------------------*/