#ifndef SCRIPTPARSERTESTCASE_H_
#define SCRIPTPARSERTESTCASE_H_

#include "ScriptParser.hpp"
#include "XMLScriptParser.hpp"
#include "Figure.h"

#include "Util.hpp"
#include "Joint.h"
#include <string>

class ScriptParserTestCase
{
public:
	static void run1()
	{
		using namespace std;
		using namespace util;
		std::cout<<"/*----------------------------------------------------------*/"<<std::endl;
		{
			std::cout << "\t\t" << "The String Type Script Parser Test Case :" << std::endl;
			ScriptParser<string> parser("script.txt");
			parser.recursBuildByIterator();
			print_tree(parser, parser.begin(), parser.end());
			typedef ScriptParser<string>::iterator myIter;
			for (myIter it = parser.begin(); it != parser.end(); ++it)
			{
				cout << (*it) << " ";
			} cout << endl;
		}
		std::cout<<"/*----------------------------------------------------------*/"<<std::endl;
	}

 	static void run2()
 	{
 		using namespace std;
 		using namespace util;
 		std::cout<<"/*----------------------------------------------------------*/"<<std::endl;
 		{
 			std::cout << "\t\t" << "The Joint Type ScriptParser Test Case :" << std::endl;
 			ScriptParser<Joint> parser("script.txt");
 			parser.recursBuildByIterator();
 			typedef ScriptParser<Joint>::iterator myIter;
 			for (myIter it = parser.begin(); it != parser.end(); ++it)
 			{
 				cout << (*it).name << " ";
 			} cout << endl;
 		}
 		std::cout<<"/*----------------------------------------------------------*/"<<std::endl;
 	}

	static void run3()
	{
		using namespace std;
		using namespace util;
	
		std::cout<<"/*----------------------------------------------------------*/"<<std::endl;
		{
			std::cout << "\t\t" << "The Joint Type XMLScriptParser Test Case :" << std::endl;
			XMLScriptParser<Joint> parser("tree.xml");
			parser.loadXMLScript();
			typedef ScriptParser<Joint>::iterator myIter;
			for (myIter it = parser.begin(); it != parser.end(); ++it)
			{
				cout << "Type = " << (*it).type << "\t";
				cout << "Name = " << (*it).name << "\n";
			} cout << endl;
		}
		std::cout<<"/*----------------------------------------------------------*/"<<std::endl;
	}

	static void run4()
	{
		using namespace std;
		using namespace util;
		std::cout<<"/*----------------------------------------------------------*/"<<std::endl;
		std::cout << "\t\t" << "The Figure Test Case :" << std::endl;

//		//原始的脚本格式
// 	  	Figure figure("script.txt");
// 	 	figure.recursBuildByIterator();
// 		figure.creatFigure();

		//使用XML作为脚本
		Figure figure("tree.xml");
		figure.loadXMLScript();
		figure.creatFigure();

	 	typedef Figure::iterator myIter;
	 
	 	for (myIter it = figure.begin(); it != figure.end(); ++it)
	 	{
	 		cout << (*it).name << "\t\t" << (*it).offset[0] << "\t" << (*it).offset[1] << "\t" << (*it).offset[2] << endl;
	 	} cout << endl;
		std::cout<<"/*----------------------------------------------------------*/"<<std::endl;
	}
};

#endif


