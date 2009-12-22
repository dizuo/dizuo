#include "OcTree.h"
#include <ctime>
#include <gl/freeglut.h>
#include "Helper.h"

OcTree::OcTree(OcNode::SpaceType area)
	: m_root(0)
{
	m_root = new OcNode(area);
	m_contentColor = Color4(0xFF0000);

	srand( (unsigned)time( NULL ) );
}

OcTree::~OcTree()
{
	recursDeleteNode( m_root );
}

// ----------------------------------------------------------------
//	Name：			OcTree::deleteNode
//	Description:	递归后序遍历删除节点
//	Return Value:	bool
// ----------------------------------------------------------------
bool OcTree::recursDeleteNode(OcNode::pointerType some)
{
	for (size_t i(0); i<OcNode::SIZE; i++)
	{
		if(some->getNode(i))
			recursDeleteNode( some->getNode(i) );
	}

	delete some;
	some = NULL;

	return true;
}

// ----------------------------------------------------------------
//	Name：			OcTree::renderNode
//	Description:	为了不遮挡
//					只能前序递归渲染以some节点为根节点的子树
//	Return Value:	bool
// ----------------------------------------------------------------
void OcTree::recursRenderNode(OcNode::pointerType some)
{
	//render the node
	Helper::renderBox( some->getBound() );
	for (int i(0); i<some->getContens().size(); i++)
	{
		int index = (rand() * i + 8) % Helper::getBufferSize( Helper::ColorStable );
		Helper::renderBox( some->getContens()[i], Color4(Helper::ColorStable[index]) );
	}
	
	for (size_t i(0); i<OcNode::SIZE; i++)
	{
		if( some->getNode(i) )
			recursRenderNode( some->getNode(i) );
	}
}
