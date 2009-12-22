#ifndef HNODE_H_
#define HNODE_H_

#include <iostream>
#include <vector>

template <class T> class HNodePointer;
template<class DataType, class NodeType = HNode<DataType> > class HTree;

template<class DataType, class NodeT = HNodePointer<DataType> >
class HNode
{
	friend class HTree<DataType>;
public:
	typedef DataType Type;
	typedef NodeT pointerType;
	typedef std::vector<pointerType> childType;

	HNode(const DataType& name_)
		: name(name_)
	{}
	~HNode(){}

 	void DoSomething()
 	{
 		//cout << "HNode" << endl;
 		std::cout << name << std::endl;
 	}

	void setData(Type& data_) { data = data_;}
	void insertChild(const pointerType& ptr){children.push_back(ptr); }
		//call the function: HNodePointer(HNode<T> * hnode)

	//access
	const Type getData() const {return data;}
	const pointerType getParent() const {return parent;}
	const pointerType getChild(size_t index) const {return children[index];}
	const childType   getChildren() const {return children;}		

	pointerType& getParent() {return parent;}
	pointerType& getChild(size_t index) {return children[index];}
	childType& getChildren() {return children;}

	//manipulator

	pointerType next;
protected:
	Type name;
	pointerType  parent;	//注意next不是指针，
							//NodeT是HNodePointer<DataType>这个类型
	childType children;
};

template <class T>
class HNodePointer
{
public:
	HNode<T> *next;
	HNodePointer(){next=0;}
	HNodePointer(HNode<T> * hnode)
	{
		//next = const_cast<HNode<T>*> (hn);
		next = hnode;
	}

	// HNode<string> x;
	// x.next = new HNode<string>();
	HNode<T>* operator=(HNode<T> * hn)
	{
		next = hn;
		return hn;
	}

	// 用于强制类型隐式转化
	// 这只有在基类里面才用到HNode这个类
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

#endif