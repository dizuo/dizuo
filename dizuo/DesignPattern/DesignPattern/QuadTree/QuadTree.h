/*************************************************************/
/**					Filename: QuadTree.H		            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  4/20/2009 	RYF - Creation							**/	
/**					Email:dizuo@126.com						**/
/**					All Rights Reserved.					**/
/*************************************************************/
#ifndef QUADTREE_H_
#define QUADTREE_H_

#include "QuadTreeNode.h"
#include "Color.h"

class QuadTree
{
	Color4		  m_contentColor;
	QuadTreeNode::pointerType m_root;
public:
	QuadTree(const QuadTreeNode::AreaType& rect);
	~QuadTree();

	//manipulator:
	bool recursDeleteNode(QuadTreeNode::pointerType some);
	void recursRenderNode(QuadTreeNode::pointerType some);

	void insertArea(const QuadTreeNode::AreaType& area) { m_root->insertArea(area); }
	QuadTreeNode::rectContainer queryArea(const QuadTreeNode::AreaType& area) { return m_root->queryArea(area); }

	void setContentColor(const Color4& color) { m_contentColor = color; }

	//accessor:
	const QuadTreeNode::pointerType getRoot()const { return m_root; };
	QuadTreeNode::pointerType& getRoot() { return m_root; }
protected:
};

//////////////////////////////////////////////////////////////////////////
//	QuadTree只是一个特殊的子树

#endif