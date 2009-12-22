#include <iostream>
using namespace std;

template<class T>
class Node
{
public:
	void toDo()
	{
		cout << "This is a " << typeid(T).name() << " type node" << endl;
	}
};

template<class DataType, class NodeType = Node<DataType> >
class List
{
	NodeType head;
public:
	typedef DataType Type;

	void Print()
	{
		head.toDo();
	}
};

int main()
{
	List<int> intList;
	intList.Print();
	
	List<float> floatList;
	floatList.Print();

	system("PAUSE");
	return 0;
}