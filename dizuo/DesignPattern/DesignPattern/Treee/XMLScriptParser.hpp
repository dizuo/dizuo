#ifndef XMLSCRIPTPARSER_H_
#define XMLSCRIPTPARSER_H_

#include <iostream>
#include <string>

#include "Tree.hpp"
#include "tinyxml.h"

namespace util {
	using namespace kptree;

	template<class DataType>
	class XMLScriptParser : public tree<DataType>
	{
		std::string file_name;	//XML Script Name
		std::string m_name;		//该多叉树的name

		//禁止默认构造函数
		XMLScriptParser(){};
	public:
		typedef DataType Type;
		XMLScriptParser(std::string name);
		virtual ~XMLScriptParser(){};

		bool loadXMLScript();
	protected:
		bool recursParse(TiXmlElement* pTreeNode, pre_order_iterator& parent);
	};

	template<class DataType>
	XMLScriptParser<DataType>::XMLScriptParser(std::string name)
		: file_name(name)
	{}

	template<class DataType>
	bool XMLScriptParser<DataType>::loadXMLScript()
	{
		TiXmlDocument doc( file_name.c_str() );
		if (!doc.LoadFile()) 
			return false;

		TiXmlHandle hDoc(&doc);
		TiXmlElement* pElem;
		TiXmlHandle hRoot(0);

		// block: name
		pElem=hDoc.FirstChildElement().Element();
		// should always have a valid root but handle gracefully if it does
		if (!pElem) 
			return false;
		m_name=pElem->Value();		//m_name = "HTree"

		// save this for later
		hRoot=TiXmlHandle(pElem);

		// block: Nodes
		
		TiXmlElement* pTreeNode = hRoot.FirstChild( "Node" ).Element();
		
		Type node;
		//读取当前节点的name: 
		const char *pName=pTreeNode->Attribute("Name");
		if (pName)
			node.name = pName;
		//读取当前节点的type: 
		const char *pType=pTreeNode->Attribute("Type");	// "ROOT"
		if (pType)
			node.type = pType;

		pre_order_iterator parent = begin();
		parent = insert(parent, node );

		// 解析XML文件
		return recursParse(  pTreeNode, parent );
	}

	template<class  DataType>
	bool XMLScriptParser<DataType>::recursParse( TiXmlElement* pTreeNode, 
												 pre_order_iterator& parent )
	{
		if ( pTreeNode == NULL ) 
			return true;

		// process block放在循环外面也是可以的，貌似区别不大

		for ( pTreeNode = pTreeNode->FirstChildElement(); 
			  pTreeNode; 
			  pTreeNode = pTreeNode->NextSiblingElement() )
		{
			{	//Process Block
				Type node;

				//读取当前节点的name: 
				const char *pName=pTreeNode->Attribute("Name");
				if (pName) node.name=pName;
				//读取当前节点的type: 
				const char *pType=pTreeNode->Attribute("Type");
				if (pType) node.type=pType;

				pre_order_iterator child = append_child(parent, node);
				//	构建一个Type对象传入append_child函数

				recursParse( pTreeNode, child );
			}
		}

		return true;
	}
}

//////////////////////////////////////////////////////////////////////////
//	4/29/2009	-	RYF
//	实现了XML脚本解析，没有必要自己写了^_^

#endif