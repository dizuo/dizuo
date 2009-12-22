#ifndef SCRIPTPARSER_H_
#define SCRIPTPARSER_H_

#include <iostream>
#include <fstream>
#include <iterator>
#include <string>

#include "Tree.hpp"

namespace util {
	using namespace kptree;
	class file_not_found{};

	template<class DataType>
	class ScriptParser : public tree<DataType>
	{
		std::string file_name;
	public:
		typedef DataType Type;

		ScriptParser(std::string name);
		virtual ~ScriptParser();

		bool recursBuildByIterator();
	protected:
		bool generateJointByIterator(std::ifstream& inFile, pre_order_iterator& parent);
		ScriptParser(){}	//默认构造函数非法
	};

	template<class DataType>
	ScriptParser<DataType>::ScriptParser(std::string _name)
		: file_name(_name)
	{}

	template<class DataType>
	ScriptParser<DataType>::~ScriptParser()
	{}

	//////////////////////////////////////////////////////////////////////////
	//利用迭代器
	template<class DataType>
	bool ScriptParser<DataType>::recursBuildByIterator()
	{
		std::ifstream in_file( file_name.c_str() );
		if ( !in_file ) {
			std::cout << "File \"" << file_name << "\" not found.\n";
			throw file_not_found(); 
			return false;
		}

		bool return_type = true;

		std::string read_string;
		std::string read_data;

		// Reading in all the roots.
		in_file >> read_string;
		if(read_string == "ROOT")
		{
			in_file >> read_data;	//Read The Name Of Joint

			pre_order_iterator parent = begin();
			parent = insert(parent, Type(read_data) );

			if(!this->generateJointByIterator(in_file, parent))
				return false;

			in_file >> read_string;
		}

		in_file.close();
		return return_type;
	}

	//递归实现
	template<class DataType>
	bool ScriptParser<DataType>::generateJointByIterator(std::ifstream& inFile,  pre_order_iterator& parent)
	{
		std::string	read_data;
		std::string	read_string;
		char	read_char;

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
			pre_order_iterator child = append_child(parent, Type(read_data));
											//	构建一个Type对象传入append_child函数
											//	掉用Type对象的构造函数 

			generateJointByIterator(inFile, child);
			inFile >> read_string;
		}
		return true;
	}
};	//namespace kptree


//  [4/1/2009 RenYaFei]
//		读取
//		存储
//		增加某一帧数据

#endif