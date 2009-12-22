/*************************************************************/
/**					Filename: OcTree.H			            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  4/21/2009 	RYF - Creation							**/	
/**					Email:dizuo@126.com						**/
/**					All Rights Reserved.					**/
/*************************************************************/
#ifndef OCTREE_H_
#define OCTREE_H_

#include "Color.h"
#include "OcNode.h"

class OcTree
{
	Color4	m_contentColor;
	OcNode*	m_root;
public:
	OcTree(OcNode::SpaceType area);
	~OcTree();

	//manipulator:
	bool recursDeleteNode(OcNode::pointerType some);
	void recursRenderNode(OcNode::pointerType some);

	void insertArea(const OcNode::SpaceType& area) { m_root->insertArea(area); }
	OcNode::boxContainer queryArea(const OcNode::SpaceType& area) { m_root->queryArea(area); }

	void setContentColor(const Color4& color) { m_contentColor = color; }

	//accessor:
	const OcNode::pointerType getRoot()const { return m_root; };
	OcNode::pointerType& getRoot() { return m_root; }
};

#endif