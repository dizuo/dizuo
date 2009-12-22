#include <ctime>
#include "QuadTree.h"
#include <gl/freeglut.h>
#include "Helper.h"

QuadTree::QuadTree(const QuadTreeNode::AreaType& rect)
	: m_root(0)
{
	m_root = new QuadTreeNode(rect);
	m_contentColor = Color4(0xFF0000);

	srand( (unsigned)time( NULL ) );
}

QuadTree::~QuadTree()
{
	recursDeleteNode( m_root );
}

// ----------------------------------------------------------------
//	Name：			QuadTree::deleteNode
//	Description:	递归后序遍历删除节点
//	Return Value:	bool
// ----------------------------------------------------------------
bool QuadTree::recursDeleteNode(QuadTreeNode::pointerType some)
{
	for (size_t i(0); i<QuadTreeNode::SIZE; i++)
		if ( some->getNode(i) )
			recursDeleteNode( some->getNode(i) );

	delete some;
	some = NULL;

	return true;
}

// ----------------------------------------------------------------
//	Name：			QuadTree::renderNode
//	Description:	为了不遮挡
//					只能前序递归渲染以some节点为根节点的子树
//	Return Value:	bool
// ----------------------------------------------------------------
void QuadTree::recursRenderNode(QuadTreeNode::pointerType some)
{
	//render the node
	Helper::renderRectangle( some->getBound() );
	for (int i(0); i<some->getContens().size(); i++)
	{
		int index = (i+5) % Helper::getBufferSize( Helper::ColorStable );
		//Helper::renderElipse( some->getContens()[i], Color4( 0x000000 ) );	绘制边框
		Helper::renderElipse( some->getContens()[i], Color4( Helper::ColorStable[index] ), false );
	}
	
	for (size_t i(0); i<QuadTreeNode::SIZE; i++)
		if ( some->getNode(i) )
			recursRenderNode( some->getNode(i) );
}
