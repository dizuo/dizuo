#ifndef HNODE_H_
#define HNODE_H_

#include <string>
#include <iostream>
#include <vector>
#include <cassert>
#include <stdexcept>

//前置申明
template <class T> class HNodePointer;
template<class DataType, class NodeType = HNode<DataType> > class HTree;

template<class DataType, class NodeT = HNodePointer<DataType> >
class HNode 
{
	friend class HTree<DataType, class NodeType>;

	typedef DataType Type;
	typedef NodeT pointerType;
	typedef std::vector<pointerType> childType;
public:
	//implement
	HNode()
	{};
	HNode(Type data_)
		: data(data_)
	{}
	HNode(const HNode<Type>& node)
		: data(node.data)
	{}

	//manipulator
	void setData(const Type& data_) { data = data_;}
	void insertChild(const pointerType& ptr) {children.push_back(ptr); }

	//access
	const Type getData() const {return data;}
	const size_t getSize() const { return children.size(); }
	const pointerType getParent() const {return parent;}
	const pointerType getChild(size_t index) const {return children[index];}
	const pointerType getFirstChild() const { return *( children.begin() ); }
	const pointerType getLastChild() const  { if(children.size()==0) throw runtime_error("Attempt To Access The Empty Vector");
											  return children[children.size()-1]; }	

	//assert或者异常抛出
	pointerType& getParent() {return parent;}
	pointerType& getChild(size_t index) {return children[index];}
	pointerType& getFirstChild() { return *( children.begin() ); }
	pointerType& getLastChild()  {  if(children.size()==0) throw runtime_error("Attempt To Access The Empty Vector");
									return children[children.size()-1]; }

protected:
	//data
	Type		data;
	pointerType	parent;
	childType	children;
};

template <class T>
class HNodePointer
{
public:
	HNode<T> *next;
	HNodePointer(){next=0;}
	HNodePointer(HNode<T> * hn)
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

//manipulator
template<class Type, class NodeType >
inline std::ostream& operator<<(std::ostream& out, NodeType node)
{
	return ( out << node.getData() );
}

//template<class Type, class NodeT>
//不能加无用的NodeT
template<class Type, class NodeType >
inline std::istream& operator>>(std::istream& in, NodeType& node)
{
	return ( in>>node.data );
}

#endif