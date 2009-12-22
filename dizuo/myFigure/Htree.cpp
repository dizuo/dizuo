#include "htree.h"
#include "util/global.h"
#include <iostream>

// ----------------------------------------------------------------
//  Name:           Htree::set_up
//  Description:    给position指定的节点增加孩子节点，
//					x,y,z为孩子节点相对于position父节点的偏移量
//  Return Value:   htree::iterator,
//					返回的iterator指向刚插入的孩子节点位置
// ----------------------------------------------------------------
Htree::iterator Htree::set_up(  Htree::iterator& position,
								std::string name, 
								float x, float y, float z )
{
	tree_node *tmp = new tree_node(name, x, y, z);

	position._node->children.push_back(tmp);
	tmp->parent = position._node;

	return Htree::iterator(tmp);
}

// ----------------------------------------------------------------
//	Name：			Htree::destroy_tree
//	Description:	后序遍历删除树结构.
//	Return Value:	void
// ----------------------------------------------------------------
void Htree::destroy_tree(Htree::tree_node *some)
{
	//后序删除
	for (unsigned i = 0; i < some->children.size(); i++)
		destroy_tree(some->children[i]);

	SAFE_DELETE(some);
}

// ----------------------------------------------------------------
//	Name：			Htree::pre_recurs_print()
//	Description:	先序遍历输出多叉树.
//	Return Value:	void
// ----------------------------------------------------------------
void Htree::pre_recurs_print( const tree_node* some, 
							  unsigned recurs_level )
{
	using namespace std;
	for (int i = 0; i<recurs_level; i++)
		cout << "  ";

	cout << some->name << endl;

	for (unsigned i = 0; i < some->children.size(); i++)
	{
		pre_recurs_print(some->children[i], recurs_level+1);
	}
}

// ----------------------------------------------------------------
//	Name：			Htree::post_recurs_print()
//	Description:	后序遍历输出多叉树.
//	Return Value:	void
// ----------------------------------------------------------------
void Htree::post_recurs_print( const tree_node* some, 
							   unsigned recurs_level )
{
	using namespace std;

	for (unsigned i = 0; i < some->children.size(); i++)
	{
		pre_recurs_print(some->children[i], recurs_level+1);
	}

	for (int i = 0; i<recurs_level; i++)
		cout << "  ";
	cout << some->name << endl;
}