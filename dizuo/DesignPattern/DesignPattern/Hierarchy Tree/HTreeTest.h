#include "HTree.hpp"
#include "Joint.h"
#include <iostream>


void printHNode( HNode<string>* some )
{
	cout << some->getData() << " ";
}

void printJoint(Joint* some)
{
	cout << some->getData() << " ";
}

class HTreeTestCase
{
public:
	static void run1()
	{
		using namespace std;
		std::cout<<"/*----------------------------------------------------------*/"<<std::endl;
		cout << "\t\t" << "The HNode Tree Test Case :" << endl;
		{
			typedef HNode<string> myNodeType;
			typedef HTree<string> myTreeType;
			typedef myTreeType::iterator myNodeTreeIter;

			myNodeType *root = new myNodeType( string("one") );
			myTreeType tree(root);

			myNodeTreeIter one(root);

			myNodeTreeIter two = tree.insertNode(one, "two");
			myNodeTreeIter three = tree.insertNode(one, "three");

			tree.insertNode(two, "apple");
			tree.insertNode(two, "banana");
			tree.insertNode(two, "peach");

			tree.insertNode(three, "china");
			tree.insertNode(three, "england");

			tree.preRecursPrint( tree.root, 1);
			tree.preTraverse( tree.root, printHNode );
			cout << endl;
		
			cout << "-----------" << endl;
			for ( myNodeTreeIter it = tree.begin(); it != tree.end(); ++it )
			{
				cout << (*it) << endl;
			}
			cout << "-----------" << endl;
		}

		std::cout<<"/*----------------------------------------------------------*/"<<std::endl;
	}

	static void run2()
	{
		std::cout<<"/*----------------------------------------------------------*/"<<std::endl;
		cout << "\t\t" << "The Joint Tree Test Case :" <<endl;
		{
			typedef HTree<string, Joint> myJointTree;
			typedef myJointTree::iterator myJointTreeIter;

			Joint* root = new Joint("one");
			myJointTree tree(root);

			myJointTreeIter one(root);

			myJointTreeIter two = tree.insertNode(one, "two");
			myJointTreeIter three = tree.insertNode(one, "three");

			tree.insertNode(two, "apple");
			tree.insertNode(two, "banana");
			tree.insertNode(two, "peach");

			tree.insertNode(three, "china");
			tree.insertNode(three, "england");

			tree.preRecursPrint( tree.root, 1);
			tree.preTraverse( tree.root, printJoint );
		}cout << endl;
		std::cout<<"/*----------------------------------------------------------*/"<<std::endl;
	}
};