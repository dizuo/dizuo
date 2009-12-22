#include "ScriptParser.hpp"

extern void printJoint(Joint* some);
extern void printHNode( HNode<string>* some );

class ScriptParserTestCase
{
public:
	static void run1()
	{
		std::cout<<"/*----------------------------------------------------------*/"<<std::endl;
		cout << "\t\t" << "The Joint Type Script Parser Test Case :" <<endl;
		{
			typedef ScriptParser<string, Joint>::iterator myIter;
			ScriptParser<string, Joint> tree("Hierarchy Tree\\script.txt");
			//tree.recursRead();
			tree.recursBuildByIterator();
			tree.preRecursPrint( tree.root, 1);
			tree.preTraverse( tree.root, printJoint );

			cout << endl << "-----------" << endl;
// 			for (myIter it = tree.begin(); ; ++it)
// 			{
// 				cout << (*it) << endl;
// 				if( it==tree.end() )
// 					break;
// 			}
			cout << "-----------" << endl;
		}
		std::cout<<"/*----------------------------------------------------------*/"<<std::endl;
	}

	static void run2()
	{
		std::cout<<"/*----------------------------------------------------------*/"<<std::endl;
		cout << "\t\t" << "The HNode Type Script Parser Test Case :" <<endl;
		{
			ScriptParser<string, HNode<string> > tree("Hierarchy Tree\\script.txt");
			tree.recursRead();
			tree.preRecursPrint( tree.root, 1);
			tree.preTraverse( tree.root, printHNode );
		}cout << endl;
		std::cout<<"/*----------------------------------------------------------*/"<<std::endl;
	}

};
