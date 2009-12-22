#ifndef HTREE_H_
#define HTREE_H_
#include "Joint.h"

class Htree
{
public:
	//////////////////////////////////////////////////////////////////////////
	//iterator
	typedef Joint tree_node;
	class iterator{
	public:
		iterator()
			: _node(0)
		{}
		iterator(tree_node *node)
			: _node(node)
		{}

		~iterator()
		{}

		std::string& operator*() const
		{
			return _node->name;
		}
		std::string* operator->() const
		{
			return &(_node->name);
		}
		tree_node* get()
		{
			return _node;
		}

		tree_node *_node;
	};

	iterator set_up( iterator& some, std::string name, float x, float y, float z );		
	void	 pre_recurs_print(const tree_node* some, unsigned recurs_level);
	void	 post_recurs_print(const tree_node* some, unsigned recurs_level);
	void	 destroy_tree(tree_node *some);
};
//  [4/1/2009 RenYaFei]
//		htree可以创建，输出，销毁层次树

//  [4/7/2009 RenYaFei]
//		试图将算法数据结构分离，迭代器知道细节
#endif
