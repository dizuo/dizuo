#include <iostream>
#include <string>
#include <vector>
using namespace std;

template<class DataType, class NodeType>
class BaseNodeMethod
{
public:
	typedef DataType	Type;
	typedef NodeType*	pointerType;

/*
	// manipulator
	virtual void insertNode(const pointerType& ptr) = 0;
	virtual void setData(const Type& data_) = 0;

	// accessor
	virtual Type		getData() = 0;

	virtual pointerType& getParent() = 0;
	virtual pointerType& getFirstChild() = 0;	
	virtual pointerType& getLastChild() = 0;
	virtual pointerType& getChild(size_t index) = 0;

	virtual const pointerType getParent() const = 0;
	virtual const pointerType getFirstChild() const = 0;
	virtual const pointerType getLastChild() const = 0;
	virtual const pointerType getChild(size_t index)const = 0;
	virtual const size_t	  getSize() const = 0;	
*/
	// some other common interface...
};

template<class DataType>
class HNode : public BaseNodeMethod<DataType, HNode<DataType> >
{
	pointerType parent;
	vector<pointerType> children;
public:
	typedef HNode<DataType>* pointerType;
	void ImHNode(){};
};


template<class DataType>
class Joint : public BaseNodeMethod<DataType, Joint<DataType> >
{
	pointerType parent;
	vector<pointerType> children;
public:
	typedef Joint<DataType>* pointerType;
	void ImJoint(){};
};

template<class NodeType>
class BaseTree
{
protected:
	// could be HNode or Joint
	NodeType root;
public:
	void InsertNode();
	void DeleteNode();
	void Output();
};

class HTree : public BaseTree<HNode<string> >
{
public:    
	void SomeNodeThing()
	{
		// root is HNode
		root.ImHNode();
	}
};

class Figure : public BaseTree<Joint<string> >
{
public:
	void SomeJointThing()
	{
		// root is Joint
		root.ImJoint();
	}
};


int main()
{

	return 0;
}