#ifndef BASEMETHOD_H_
#define BASEMETHOD_H_

template<class DataType, class NodeType>
class BaseNodeMethod
{
public:
	typedef DataType	Type;
	typedef NodeType*	pointerType;

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

	// some other common interface...
};

#endif