#ifndef HTREE_H_
#define HTREE_H_
#include <stdexcept>
#include "HNode.hpp"

#define SAFE_DELETE(p) {if(p){delete p; p = 0;}}

// 数据结构NodeType
template<class DataType, class NodeType>
class HTree
{
	typedef DataType Type;
	typedef NodeType treeNode;
	typedef void (*funcType)(treeNode*);
public:
	HTree()
		: root(0)
	{}
	HTree(treeNode* node)
		: root(node)
	{}
	~HTree()
	{
		destroyTree(root);
	}

 	class iterator_base;
 	class pre_order_iterator;
//   	class post_order_iterator;
  	class level_order_iterator;
	typedef pre_order_iterator iterator;

	//默认先序
	class iterator_base{
	public:
		typedef treeNode tree_node;
		typedef iterator_base my_type;

		iterator_base()
			: node(0)
			, seq_stack(0)
			, top(0)
		{
			if(!seq_stack)
				seq_stack = new unsigned[20];
		}
		iterator_base(tree_node *node_)
			: node(node_)
			, seq_stack(0)
			, top(0)
		{
			if(!seq_stack)
				seq_stack = new unsigned[20];
		}

		~iterator_base()
		{
			SAFE_DELETE(seq_stack);
		}

		const Type operator*() const
		{
			return node->getData();
		}
		const Type* operator->() const
		{
			return &(node->getData());
		}
		tree_node* get()
		{
			return node;
		}
		
		virtual my_type&	operator++()=0;
		virtual my_type&	operator--()=0;
//		virtual iterator_base&  operator+=(unsigned int)=0;
//		virtual iterator_base&  operator-=(unsigned int)=0;
		
		tree_node *node;
	protected:
		unsigned* seq_stack;	//数组栈协助遍历
		unsigned top;
	};

	class pre_order_iterator : public iterator_base
	{
	public:
		pre_order_iterator() 
			: iterator_base(0)
		{}

		pre_order_iterator(tree_node *tn)
			: iterator_base(tn)
		{
			setParent_();
		}

		pre_order_iterator(const iterator_base &other)
			: iterator_base(other.node)
		{
			setParent_();
		}
		
		bool operator==(const pre_order_iterator& other) const
		{
			if(other.node==node) return true;
			else return false;
		}

		bool operator!=(const pre_order_iterator& other) const
		{
			if(other.node!=node) return true;
			else return false;
		}

		virtual my_type&  operator++()
		{
			if( node==0 )
				throw runtime_error("Attempt To Access The Empty Vector");

			if( node->getSize()!=0 )
			{
				parent_ = node;	//更新parent
				node = node->getFirstChild();
				seq_stack[++top] = 0;	//进栈
			}
			else
			{
				seq_stack[top]++;	//指向下一个孩子
				//已经遍历完兄弟节点 返回上一层.
				while ( seq_stack[top] >= parent_->getSize() )
				{
					top--;			//出栈，返回上一层
					if( top==0 )	//栈空
					{
						node = 0;
						return *this;	
					}
					seq_stack[top]++;	//转向下个节点

					node = parent_;
					if(node==0)
						return *this;
					parent_ = parent_->getParent();
					if(parent_==0)
						return *this;
				}
				//获得兄弟节点
				node = parent_->getChild( seq_stack[top] );
			}
			return *this;
		}

		virtual my_type& operator--()
		{
			return *this;
		}

		tree_node* parent_;
	private:
		void setParent_()
		{
			parent_ = 0;
			if(node==0)
				return;
			if(node->getParent()!=0)
				parent_ = node->getParent();
		};
	};
	
	class post_order_iterator : public iterator_base
	{
	public:
		post_order_iterator() 
			: iterator_base(0)
		{}

		post_order_iterator(tree_node *tn)
			: iterator_base(tn)
		{}

		post_order_iterator(const iterator_base &other)
			: iterator_base(other.node)
		{}

		bool operator==(const post_order_iterator& other) const
		{
			if(other.node==node) return true;
			else return false;
		}

		bool operator!=(const post_order_iterator& other) const
		{
			if(other.node!=node) return true;
			else return false;
		}

		virtual my_type&  operator++()
		{
			return *this;
		}
		virtual my_type& operator--()
		{
			return *this;
		}
	protected:
		//根绝当前的some求得其后续遍历的第一个孩子节点
		//也要对seq_stack操作
		//与operator++配合使用
		tree_node* post_begin(tree_node* some)
		{

		};
	};

	class level_order_iterator : public iterator_base
	{};

	iterator begin() const
	{
		if( root==0 )
			throw runtime_error("Attempt To Access The Empty Vector");

		return iterator(root);
	}

	//STL中的前闭后开: [begin, end)
	//只是遵守和operator++函数的约定即可.
	iterator end() const
	{
		if( root==0 ) 
			throw runtime_error("Attempt To Access The Empty Vector");
		
		return iterator(0);
	}
	
	//最下层的最后一个孩子
	post_order_iterator begin_post() const
	{
		treeNode p = root;
		while (p->getFirstChild()->getSize()>0)
		{
			p = p->getFistChild();
		}
		return post_order_iterator(p);
	}
	
	//和post_order_iterator::operator++()协商好的NULL
	post_order_iterator end_post() const
	{}

	//根节点
	level_order_iterator begin_level() const
	{}
	
	//和level_order_iterator::operator++()协商好的NULL
	level_order_iterator end_level() const
	{}

	//////////////////////////////////////////////////////////////////////////
	iterator insertNode( iterator& position, const Type& data)
	{
		treeNode *tmp = new treeNode(data);

		position.node->insertChild(tmp);
		tmp->getParent() = position.node;

		return iterator(tmp);
	}
	
	//有可能修改node，所以没有必要使用const
	void preTraverse(treeNode *some, funcType doSomething )
	{
		doSomething(some);

		for (unsigned i = 0; i < some->getSize(); i++)
			preTraverse( some->getChild(i), doSomething );
	}

	void postTraverse(const treeNode *some, funcType doSomething )
	{
		for (unsigned i = 0; i < some->getSize(); i++)
			postTraverse( some->getChild(i), doSomething );

		doSomething(some);
	}

	void destroyTree(treeNode* some)
	{
		//后序删除
		//postTraverse(root, &HTree<DataType>::deleteNode);
		for (unsigned i = 0; i < some->getSize(); i++)
			destroyTree( some->getChild(i) );

		SAFE_DELETE(some);
	};

	void preRecursPrint(treeNode *some, unsigned recurs_level)
	{
		using namespace std;
		for (size_t i = 0; i<recurs_level; i++)
			cout << "  ";

 		cout << some->getData() << endl;
		//cout << *some << endl;
 
 		for (unsigned i = 0; i < some->getSize(); i++)
 		{
 			preRecursPrint(some->getChild(i), recurs_level+1);
 		}
	}

	treeNode *root;
protected:
};
//  [4/1/2009 RenYaFei]
//		htree可以创建，输出，销毁层次树

// [4/7/2009 RenYaFei]
//		使用模板化

// [4/8/2009 RenYaFei]
//		for循环能不能for_each替代

// [4/8/2009 RenYaFei]
//		命名规则：函数多个单词：preRecursPrint()
//		命名规则：变量一律小写，多个单词，下划线隔开

#endif