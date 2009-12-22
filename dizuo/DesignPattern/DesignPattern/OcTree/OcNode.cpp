#include "OcNode.h"
#include "Vec3.hpp"

#include "Helper.h"
#include <iostream>

// ----------------------------------------------------------------
//	Name：			OcNode::OcNode
//	Description:	重载构造函数，通过box构造一个八叉树
//	Return Value:	...
// ----------------------------------------------------------------
OcNode::OcNode(const OcNode::SpaceType& box)
	: m_bounds(box)
{
	for (size_t i(0); i<OcNode::SIZE; i++) m_nodes[i] = NULL;
	m_contents.clear();
}

// ----------------------------------------------------------------
//	Name：			OcNode::createSubNodes
//	Description:	增加八个孩子节点
//	Return Value:	void
// ----------------------------------------------------------------
void OcNode::createSubNodes()
{
	Vec3f size = m_bounds.getSize();
	float x = size.x();
	float y = size.y();
	float z = size.z();

	if ( (x * y * z) <= 10 )	//此处的return可能导致m_nodes[]为NULL
		return;

	Vec3f min = m_bounds.getMin();
	Vec3f max = m_bounds.getMax();
	Vec3f half = (max - min) / 2.0;
	Vec3f halfx; halfx.x() = x / 2.0;	
	Vec3f halfy; halfy.y() = y / 2.0;
	Vec3f halfz; halfz.z() = z / 2.0;

	//	Octree subdivides in this order:
	//						N
	//			  			/
	//		    <-4--------/------5
	//	 W______ /|_______/______/|__________E
	//			/ |		 /|	    / |	
	//	       6--------/-|-----7 |
	//		   |  |	   /| |	   |  |
	//		   |  |	  S	| |	   |  |	
	//		   |  |		|	   |  |
	//		   | 0/------------|--/1
	//		   | /			   | /
	//		   |/______________|/
	//		   2			   3
	//	create bounding boxes for children
	m_nodes[DOWN_NW] = new OcNode( Box3f(min, min + half) );
	m_nodes[DOWN_NE] = new OcNode( Box3f( min + halfx, max - half + halfx) );
	m_nodes[DOWN_SW] = new OcNode( Box3f(min + halfz, min + half + halfz) );
	m_nodes[DOWN_SE] = new OcNode( Box3f(min + halfx + halfz, max - halfy) );
	m_nodes[UP_NW] = new OcNode( Box3f(min + halfy, max - halfx - halfz) );
	m_nodes[UP_NE] = new OcNode( Box3f(min + halfy + halfx, max - halfz) );
	m_nodes[UP_SW] = new OcNode( Box3f(min + halfy + halfz, max - halfx) );
	m_nodes[UP_SE] = new OcNode( Box3f(min + half, max) );
}

// ----------------------------------------------------------------
//	Name：			OcNode::countNodes
//	Description:	返回子树的所有节点个数
//						孩子节点 + content个数
//	Return Value:	int
// ----------------------------------------------------------------
int OcNode::countNodes()
{
	int count = 0;

	for (size_t i(0); i<OcNode::SIZE; i++) {
		if(m_nodes[i])
			count += m_nodes[i]->countNodes();
	}

	count += m_contents.size();
	return count;
}

// ----------------------------------------------------------------
//	Name：			OcNode::subTreeContents
//	Description:	返回父节点在内的所有孩子节点的content内容
//	Return Value:	vector<Box3f>
// ----------------------------------------------------------------
OcNode::boxContainer OcNode::subTreeContents()
{
	OcNode::boxContainer results;

	for (size_t i(0); i<OcNode::SIZE; i++)
		if(m_nodes[i])
			Helper::myAddRange( results, m_nodes[i]->subTreeContents() );

	Helper::myAddRange( results, m_contents );
	return results;
}

// ----------------------------------------------------------------
//	Name：			OcNode::insertArea
//	Description:	主要功能函数
//					将一个item插入到某个节点中
//					item可以是sphere，
//					或其他形状，程序里只是实现了Box.
//	Return Value:	void
// ----------------------------------------------------------------
void OcNode::insertArea(const OcNode::SpaceType& item)
{
	// if the item is not contained in this quad, there's a problem
	if ( !m_bounds.contain(item) )
	{
		std::cout << ("feature is out of the bounds of this quadtree node") << std::endl;
		return;
	}

	// if the sub nodes are null create them. may not be sucessfull: see below
	// we may be at the smallest allowed size in which case the sub nodes will not be created
	if ( countNodes() == 0 )
		createSubNodes();

	// for each subnode:
	// if the node contains the item, add the item to that node and return
	// this recurses into the node that is just large enough to fit this item
	for(size_t i(0); i<OcNode::SIZE; i++)
	{
		if( m_nodes[i] == NULL )
			continue;
		if ( m_nodes[i]->getBound().contain(item) )
		{
			m_nodes[i]->insertArea(item);
			return;
		}
	}

	// if we make it to here, either
	// 1) none of the subnodes completely contained the item. or
	// 2) we're at the smallest subnode size allowed 
	// add the item to this node's contents.
	m_contents.push_back(item);
}

// ----------------------------------------------------------------
//	Name：			OcNode::queryArea
//	Description:	返回包含相交区域或者与查询区域相交的所有矩形区域
//	Return Value:	vector<Box3f>
// ----------------------------------------------------------------
OcNode::boxContainer OcNode::queryArea(const OcNode::SpaceType& area)
{
	// create a list of the items that are found
	OcNode::boxContainer results;

	// this quad contains items that are not entirely contained by
	// it's four sub-quads. Iterate through the items in this quad 
	// to see if they intersect.
	for (size_t i(0); i<m_contents.size(); i++)
	{
		if( m_contents[i].intersect(area) )
			results.push_back( m_contents[i] );
	}

	for (size_t i(0); i<OcNode::SIZE; i++)
	{
		//终端节点跳过
		if ( m_nodes[i]->countNodes() == 0 )
			continue;

		// Case 1: search area completely contained by sub-quad
		// if a node completely contains the query area, go down that branch
		// and skip the remaining nodes (break this loop)
		if ( m_nodes[i]->getBound().contain(area) )
		{
			//results.AddRange(node.Query(queryArea));
			Helper::myAddRange( results, m_nodes[i]->queryArea(area) );
			break;
		}

		// Case 2: Sub-quad completely contained by search area 
		// if the query area completely contains a sub-quad,
		// just add all the contents of that quad and it's children 
		// to the result set. You need to continue the loop to test 
		// the other quads
		if ( area.contain(m_nodes[i]->getBound()) )
		{
			//增加subTreeContents()函数
			//results.AddRange(node.SubTreeContents);
			Helper::myAddRange( results, m_nodes[i]->subTreeContents() );
			continue;
		}

		// Case 3: search area intersects with sub-quad
		// traverse into this quad, continue the loop to search other
		// quads
		if ( m_nodes[i]->getBound().intersect(area) )
		{
			//results.AddRange(node.Query(queryArea));
			Helper::myAddRange( results, m_nodes[i]->queryArea(area) );
		}
	}

	return results;
}

//////////////////////////////////////////////////////////////////////////
//在OnMouseUp函数中调用insertArea( 在OnMouseDown函数中构造的一个点击区域 )