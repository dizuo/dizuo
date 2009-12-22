#include <iostream>
#include <string>
#include <vector>
using namespace std;

template <class T> class HNodePointer;

template<class DataType, class NodeT = HNodePointer<DataType> >
class HNode
{
public:
	typedef DataType Type;
	typedef NodeT pointerType;

	HNode(const DataType& name_)
		: name(name_)
	{}
	~HNode(){}

	void DoSomething()
	{
		//cout << "HNode" << endl;
		std::cout << name << std::endl;
	}
	pointerType next;
	Type name;
};

template <class T>
class HNodePointer
{
public:
	HNode<T> *next;
	HNodePointer(){next=0;}
	HNodePointer(const HNode<T> * hn)
	{
		next = hn;
	}

	HNode<T>* operator=(HNode<T> * hn)
	{
		next = hn;
		return hn;
	}

	//用于强制类型隐式转化
	//这只有在基类里面才用到HNode这个类
	operator HNode<T>*()
	{
		return (next);
	}
	HNode<T>* operator->()
	{
		return (next);
	}
	HNode<T>& operator*()
	{
		return *(next);
	}
	HNode<T>** operator&()
	{
		return &(next);
	}
};

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

int main()
{
  	Joint joint("Joint");
  	joint.DoSomething();
 	joint.next = new Joint("Joint->next");
 	joint.next->DoSomething();	
 	delete joint.next;
 
   	HNode<string> node("HNode");
  	node.DoSomething();
  	node.next = new HNode<string>("HNode->next");
	node.next->DoSomething();
  	delete node.next;

	return 0;
}