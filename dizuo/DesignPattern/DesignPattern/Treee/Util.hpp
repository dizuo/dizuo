/*
	A collection of miscellaneous utilities that operate on the templated 
	tree.hh class.

	(At the moment only a printing utility.
	Linda Buisman, linda.buisman@studentmail.newcastle.edu.au)

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; version 2.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef tree_util_hpp_
#define tree_util_hpp_

#include <iostream>
#include "Tree.hpp"

namespace util {
	using namespace kptree;

	// 函数申明
	template<class T>
	void print_tree_bracketed(const tree<T>& t, std::ostream& str=std::cout);

	template<class T>
	void print_subtree_bracketed(const tree<T>& t, typename tree<T>::iterator iRoot, 
		std::ostream& str=std::cout);

	template<typename Type>
	void print_tree(const tree<Type>& tr, typename tree<Type>::pre_order_iterator it, typename tree<Type>::pre_order_iterator end);
	
	template<typename Type>
	void print_tree_post(const tree<Type>& tr, typename tree<Type>::post_order_iterator it, typename tree<Type>::post_order_iterator end);
	
	template<typename Type>
	void print_tree_rev(const tree<Type>& tr, typename tree<Type>::pre_order_iterator it, typename tree<Type>::pre_order_iterator end);

	template<typename Type>
	void print_tree_rev_post(const tree<Type>& tr, typename tree<Type>::post_order_iterator it, typename tree<Type>::post_order_iterator end);


	// 函数定义
	// Iterate over all roots (the head) and print each one on a new line
	// by calling printSingleRoot.
	template<class T>
	void print_tree_bracketed(const tree<T>& t, std::ostream& str) 
	{
		int headCount = t.number_of_siblings(t.begin());
		int headNum = 0;
		for(typename tree<T>::sibling_iterator iRoots = t.begin(); iRoots != t.end(); ++iRoots) {
			print_subtree_bracketed(t,iRoots,str);
			if (headNum <= headCount - 1) {
				str << std::endl;
			}
		}
	}


	// 方括号内的
	// Print everything under this root in a flat, bracketed structure.
	template<class T>
	void print_subtree_bracketed(const tree<T>& t, typename tree<T>::iterator iRoot, std::ostream& str) 
	{
		if(t.begin() == t.end()) return;
		if (t.number_of_children(iRoot) == 0) {
			str << *iRoot; 
		}
		else {
			// parent
			str << *iRoot;
			str << "(";
			// child1, ..., childn
			int siblingCount = t.number_of_siblings(t.begin(iRoot));
			int siblingNum;
			typename tree<T>::sibling_iterator iChildren;
			for (iChildren = t.begin(iRoot), siblingNum = 0; iChildren != t.end(iRoot); ++iChildren, ++siblingNum) {
				// recursively print child
				print_subtree_bracketed(t,iChildren,str);
				// comma after every child except the last one
				if (siblingNum != siblingCount - 1 ) {
					str << ", ";
				}
			}
			str << ")";
		}
	}

	template<typename Type>
	void print_tree(const tree<Type>& tr, typename tree<Type>::pre_order_iterator it, typename tree<Type>::pre_order_iterator end)
	{
		if(!tr.is_valid(it)) return;
		int rootdepth=tr.depth(it);
		std::cout << "-----" << std::endl;
		while(it!=end) {
			for(int i=0; i<tr.depth(it)-rootdepth; ++i) 
				std::cout << "  ";
			std::cout << (*it) << std::endl << std::flush;
			++it;
		}
		std::cout << "-----" << std::endl;
	}
	
	template<typename Type>
	void print_tree_post(const tree<Type>& tr, typename tree<Type>::post_order_iterator it, typename tree<Type>::post_order_iterator end)
	{
		int rootdepth=tr.depth(it);
		std::cout << "-----" << std::endl;
		while(it!=end) {
			for(int i=0; i<tr.depth(it)-rootdepth; ++i) 
				std::cout << "  ";
			std::cout << (*it) << std::endl << std::flush;
			++it;
		}
		std::cout << "-----" << std::endl;
	}

	template<typename Type>
	void print_tree_rev(const tree<Type>& tr, typename tree<Type>::pre_order_iterator it, typename tree<Type>::pre_order_iterator end)
	{
		--it;
		int rootdepth=0;//tr.depth(it);
		std::cout << "-----" << std::endl;
		while(1==1) {
			for(int i=0; i<tr.depth(it)-rootdepth; ++i) 
				std::cout << "  ";
			std::cout << (*it) << std::endl;
			if(it==end) break;
			--it;
		}
		std::cout << "-----" << std::endl;
	}
	
	template<typename Type>
	void print_tree_rev_post(const tree<Type>& tr, typename tree<Type>::post_order_iterator it, typename tree<Type>::post_order_iterator end)
	{
		--it;
		int rootdepth=0;//tr.depth(it);
		std::cout << "-----" << std::endl;
		while(1==1) {
			for(int i=0; i<tr.depth(it)-rootdepth; ++i) 
				std::cout << "  ";
			std::cout << (*it) << std::endl;
			if(it==end) break;
			--it;
		}
		std::cout << "-----" << std::endl;
	}

};

//////////////////////////////////////////////////////////////////////////
//	可以使用函数指针将这些函数的操作一般化
//  [4/12/2009 RenYaFei]
#endif
