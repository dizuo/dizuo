#include <algorithm>
#include <iostream>
#include <string>
#include <list>
using namespace std;

typedef struct tagMember 
{ 
	char *name; 
	char *group; 
}Member; 
Member people[]=
{ 
	{"张三","一班"}, 
	{"李四","一班"}, 
	{"王五","二班"}, 
	{"赵六","三班"}, 
	{"赵七","三班"}, 
}; 

typedef list<char*> ListType;
typedef struct ListNode
{
	char* groupName;
	ListType *next;
}NodeType;

typedef list<NodeType*> myList;
typedef myList::iterator myIter;

myIter myFind(myList& mylist, char* dest)
{
	myIter it;
	for (it=mylist.begin(); it != mylist.end(); it++)
	{
		if( (*it)->groupName == dest)
			return it;
	}
	return it;
}
int main()
{
	myList classInfo;

	NodeType* pNode;
	//插入节点
	for(size_t i=0; i<5; i++)
	{
		//创建一个索引节点
		myIter it = myFind(classInfo, people[i].group);
		if( it!= classInfo.end() )	//已存在
		{
			//存储name
			(*it)->next->push_back( people[i].name );
		}
		else	//创建新的NodeType节点
		{
			pNode = new NodeType;
			pNode->groupName = people[i].group;
			pNode->next = new ListType;	//创建list

			//存储当前的name
			pNode->next->push_back( people[i].name );
			classInfo.push_back( pNode );
		}
	}
	
	//输出：
	for (myIter it=classInfo.begin(); it != classInfo.end(); it++)
	{
		cout << (*it)->groupName << ": ";
		for ( ListType::iterator bg=(*it)->next->begin(); 
			  bg != (*it)->next->end(); bg++ )
		{
			cout << (*bg) << "\t";
		}
		cout << endl;
	}

	system("PAUSE");
	return 0;
}
// 
// 一班: 张三      李四
// 二班: 王五
// 三班: 赵六      赵七