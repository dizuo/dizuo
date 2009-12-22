#include <iostream>
#include <string>
#include <vector>

#include "HNode.hpp"
#include "HTree.hpp"

using namespace std;

class Joint : public HNode<string, Joint*>
{
public:
	Joint(string name_)
		: HNode(name_)
	{}
	void DoSomething()
	{
		//cout << "Joint" << endl;
		cout << name << endl;
	}
};

void fun(string & str0)
{
	cout<<"please input backspace"<<endl;
	char ch;
	cin>>ch;

	while(ch==0x08)
	{
		str0.erase(str0.end());
		cout<<str0<<endl;
		cin>>ch;
	}
}

int main()
{
//  Joint joint("Joint");
// 	joint.DoSomething();
// 	joint.next = new Joint("Joint->next");
// 	joint.next->DoSomething();	
// 	delete joint.next;
// 
// 	HNode<string> node("HNode");
// 	node.DoSomething();
// 	node.next = new HNode<string>("HNode->next");
// 	delete node.next;
	//HNode<string> *root = new HNode<string>("HNode");

	typedef HNode<string> myNodeType;
	typedef HTree<string> myTreeType;
	typedef myTreeType::iterator myHTreeIter;

	myNodeType* root = new myNodeType( string("one") );
	myTreeType tree(root);

	myHTreeIter one(root);
	
	myHTreeIter two = tree.insertNode(one, "two");
	myHTreeIter three = tree.insertNode(one, "three");

	tree.insertNode(two, "apple");
	tree.insertNode(two, "banana");
	tree.insertNode(two, "peach");

	tree.insertNode(three, "china");
	tree.insertNode(three, "england");

	system("PAUSE");
	return 0;
}