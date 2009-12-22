/*************************************************************/
/**					Filename: OcNode.H			            **/
/**					Version:  Common                        **/
/**   History:                                              **/
/**	  4/21/2009 	RYF - Creation							**/	
/**					Email:dizuo@126.com						**/
/**					All Rights Reserved.					**/
/*************************************************************/
#ifndef OCNODE_H_
#define OCNODE_H_

#include "Box3.hpp"
#include <vector>
#include <cassert>

class OcNode
{
public:
	typedef OcNode* pointerType;
	typedef Box3f SpaceType;
	typedef std::vector<SpaceType> boxContainer;
	static const size_t SIZE = 8;
	
	enum DIRECTION{ DOWN_NW = 0, DOWN_NE, DOWN_SW, DOWN_SE, UP_NW, UP_NE, UP_SW, UP_SE };
	OcNode(const SpaceType& box);

	//manipulator:
	void	setBounds(const SpaceType& box){ m_bounds = box; }
	SpaceType&	getBound() { return m_bounds; }
	pointerType& getNode(DIRECTION dir) {return m_nodes[dir];}
	pointerType& getNode(int index) {assert(index >= 0 && index <= SIZE-1); return m_nodes[index];}	

	int countNodes();
	void createSubNodes();
	void insertArea(const SpaceType& item);
	boxContainer queryArea(const SpaceType& area);
	boxContainer subTreeContents();

	//accessor:
	const boxContainer getContens()const { return m_contents; }
	const SpaceType getBounding() const { return m_bounds; }
	const pointerType getNode(DIRECTION dir) const{return m_nodes[dir];}
	const pointerType getNode(int index)const {assert(index >= 0 && index <= SIZE-1); return m_nodes[index];}

private:
	SpaceType m_bounds;
	boxContainer m_contents;	//也可以是sphere的...
	pointerType m_nodes[8];

};

//////////////////////////////////////////////////////////////////////////
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

// vec 0 == m_bounding.getMin();
// vec 7 == m_bounding.getMax();	

#endif