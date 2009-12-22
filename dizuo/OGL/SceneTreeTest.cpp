#include <windows.h>
#include <crtdbg.h>
#include <malloc.h>
#include <iostream>
#include <iomanip>
#include <typeinfo>
#include <vector>
using namespace std;

static int id = 0;
class Object
{
public:
	Object()
	{
		id++;
		ID = id;
	}
	~Object(){
		id--;
	}

	void Draw(){
		cout << "The object ID is " << ID << endl;
	}
public:
	int ID;
};


class Transform
{
public:
	Transform()
		:matrix(2)
	{}
	~Transform(){
		for (vector<Transform*>::iterator it = TransVec.begin(); it != TransVec.end(); it++)
		{
			if(*it)
			{
				delete (*it);
				cout << "delete..." << endl;
				(*it) = NULL;
			}
		}
		for (vector<Object*>::iterator it = ObjVec.begin(); it != ObjVec.end(); it++)
		{
			if(*it)
			{
				delete (*it);
				cout << "delete..." << endl;
				(*it) = NULL;
			}
		}
	}
	void Traverse(){
		for (vector<Object*>::iterator it = ObjVec.begin(); it != ObjVec.end(); it++)
		{
			cout << "The transform is " << this->matrix << "...";
			(*it)->Draw();
		}
		
		for (vector<Transform*>::iterator it = TransVec.begin(); it != TransVec.end(); it++)
		{
			int tem = (*it)->matrix;
			(*it)->matrix *= this->matrix;
			(*it)->Traverse();
			(*it)->matrix = tem;
		}
	}

	void addChild(Object* child){
		ObjVec.push_back( child );
	}

	// 累积变换 ： 不太好
	void addChild(Transform* child){
		//child->matrix *= this->matrix;
		TransVec.push_back( child );
	}

	int		matrix;

private:
	vector<Transform*>	TransVec;
	vector<Object*>		ObjVec;
};


class SceneTree
{
public:
	SceneTree(){}
	~SceneTree()
	{
		for (vector<Transform*>::iterator it = TransVec.begin(); it != TransVec.end(); it++)
		{
			if(*it)
			{
				delete (*it);
				cout << "deleting..." << endl;
				(*it) = NULL;
			}
		}
		TransVec.clear();
	}

	void addChild(Transform* child){
		TransVec.push_back( child );
	}

	void Traverse(){
		for (vector<Transform*>::iterator it = TransVec.begin(); it != TransVec.end(); it++)
		{
			(*it)->Traverse();
		}
	}
protected:
	vector<Transform*> TransVec;
};


int main()
{
	int tmpFlag = _CrtSetDbgFlag( _CRTDBG_REPORT_FLAG ); 
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF; 
	_CrtSetDbgFlag( tmpFlag );

	SceneTree tree;

	Transform* tran1, *tran2, *tran3;
	tran1 = new Transform;
	tran2 = new Transform;
	Object* obj1 = new Object;
	tran2->addChild( obj1 );

	tran3 = new Transform;
	Object* obj2 = new Object;
	tran3->addChild( obj2 );

	Object* obj3 = new Object;
	tran1->addChild( obj3 );
	tran1->addChild(tran2);
	tran1->addChild(tran3);
	//tran1->Traverse();

	tree.addChild( tran1 );
	tree.Traverse();

	// matrix update 问题...
 	tran2->matrix++;
 	tree.Traverse();

	return 0;
}
