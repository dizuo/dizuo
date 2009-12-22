#ifndef BVHEDITER_H_
#define BVHEDITER_H_

#include <iostream>
#include <fstream>
#include <iterator>
#include <string>

#include "Joint.h"
#include "HTree.hpp"

using namespace std;

template<class DataType, class NodeType>
class ScriptParser : public HTree<DataType, NodeType>
{
	typedef NodeType part;
	typedef DataType Type;

	std::string file_name;
public:
	ScriptParser(std::string name);
	virtual ~ScriptParser();

	bool recursRead();
	//void recursWrite(ofstream &outFile);
	bool recursBuildByIterator();
protected:
	bool generateJoint(std::ifstream& inFile, part* &node,  part* parent);
	bool generateJointByIterator(std::ifstream& inFile, iterator& parent);
};

class file_not_found{};

template<class DataType, class NodeType>
ScriptParser<DataType, NodeType>::ScriptParser(std::string _name)
	: file_name(_name)
{}

template<class DataType, class NodeType>
ScriptParser<DataType, NodeType>::~ScriptParser()
{}

template<class DataType, class NodeType>
bool ScriptParser<DataType, NodeType>::recursRead()
{
	ifstream in_file( file_name.c_str() );
	if ( !in_file ) {
		cout << "File \"" << file_name << "\" not found.\n";
		throw file_not_found(); 
		return false;
	}

	bool return_type = true;

	string read_string;
	//double read_double;

	// Reading in all the roots.
	in_file >> read_string;
	if(read_string == "ROOT")
	{
		root = new part;

		if(!this->generateJoint(in_file, root, NULL))
			return false;

		in_file >> read_string;
	}

	in_file.close();
	return return_type;
}

//递归实现
template<class DataType, class NodeType>
bool ScriptParser<DataType, NodeType>::generateJoint(ifstream& inFile, part* &node,  part* parent)
{
	Type	read_data;
	string	read_string;
	char	read_char;
	part	*next_node;

	// Read the name of the Joint
	inFile >> read_data;
	node->setData(read_data);

	// Next we check to see if we get a '{'
	inFile >> read_char;
	if(read_char != '{') 
		return false;   //error

	// Now we have a loop so that we continue to create links in
	// the correct hierarchical location:
	inFile >> read_string;
	while(read_string == "JOINT" || read_string == "END")
	{
		next_node = new part;

		node->insertChild(next_node);
		next_node->getParent() = node;

		generateJoint(inFile, next_node, node);
		inFile >> read_string;
	}
	return true;
}

//////////////////////////////////////////////////////////////////////////
//利用迭代器
template<class DataType, class NodeType>
bool ScriptParser<DataType, NodeType>::recursBuildByIterator()
{
	ifstream in_file( file_name.c_str() );
	if ( !in_file ) {
		cout << "File \"" << file_name << "\" not found.\n";
		throw file_not_found(); 
		return false;
	}

	bool return_type = true;

	string read_string;
	Type   read_data;

	// Reading in all the roots.
	in_file >> read_string;
	if(read_string == "ROOT")
	{
		in_file >> read_data;	//Read The Name Of Joint
		root = new part(read_data);
		iterator parent(root);	//记录根节点位置

		if(!this->generateJointByIterator(in_file, parent))
			return false;

		in_file >> read_string;
	}

	in_file.close();
	return return_type;
}

//递归实现
template<class DataType, class NodeType>
bool ScriptParser<DataType, NodeType>::generateJointByIterator(ifstream& inFile,  iterator& parent)
{
	Type	read_data;
	string	read_string;
	char	read_char;
	part	*next_node;

	// Next we check to see if we get a '{'
	inFile >> read_char;
	if(read_char != '{') 
		return false;   //error

	// Now we have a loop so that we continue to create links in
	// the correct hierarchical location:
	inFile >> read_string;
	while(read_string == "JOINT" || read_string == "END")
	{
		inFile >> read_data;	//Read The Name Of Joint
		iterator child = insertNode(parent, read_data);

		generateJointByIterator(inFile, child);
		inFile >> read_string;
	}
	return true;
}


//  [4/1/2009 RenYaFei]
//		读取
//		存储
//		增加某一帧数据

#endif