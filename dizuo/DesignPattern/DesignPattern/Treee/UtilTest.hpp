#include "Util.hpp"

class TreeUtilTestCase
{
public:
	static void run()
	{
		using namespace util;
		using namespace std;
		using namespace kptree;

		std::cout<<"/*----------------------------------------------------------*/"<<std::endl;
		{
			std::cout << "\t\t" << "The Tree Util Test Case :" << std::endl;
			tree<string> tr;
			tree<string>::iterator top, one, two, loc, banana;

			top=tr.begin();
			one=tr.insert(top, "one");
			two=tr.append_child(one, "two");
			tr.append_child(two, "apple");
			banana=tr.append_child(two, "banana");
			tr.append_child(banana,"cherry");
			tr.append_child(two, "peach");
			tr.append_child(one,"three");

			cout << "\t\tPre - order traverse " << endl;
			util::print_tree(tr, tr.begin(), tr.end());

			// 以括号的形式输出
			cout << "\t\tPre - order traverse bracketed " << endl;
			util::print_subtree_bracketed(tr, tr.begin(), std::cout);
			cout << endl;
			util::print_tree_bracketed(tr, std::cout);

			cout << endl << "\t\tPost - order traverse " << endl;
			util::print_tree_post(tr, tr.begin_post(), tr.end_post());

			// 输出sibling
			tree<string>::sibling_iterator bg1 = tr.begin(tr.begin());
			tree<string>::sibling_iterator end1 = tr.end(tr.end());
			while (bg1 != end1)
			{
				cout << (*bg1) <<" ";
				++bg1;
			} cout << endl;
		}
		std::cout<<"/*----------------------------------------------------------*/"<<std::endl;
	}
};