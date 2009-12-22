#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
using namespace std;

class Anima
{
public:
	Anima()
		:m_x(0), m_y(0)
	{}
	int m_x;
	int m_y;
	virtual void Draw()	const
	//void Draw()
	{
		cout<<m_x<<" "<<m_y<<endl;
	}
};

class Prop : public Anima
{
public:
	//virtual Draw() const{};
	bool IsEatByMan(int i)
	{
		if(i==0)
		{
			cout<<" Dead"<<endl;	
			return true;
		}
		else
			cout<<" living"<<endl;
		
		return false;
	}
	virtual DoSomething() const=0;
};

class Bomb : public Prop
{
public:
	Bomb()
	{
		m_x=3;
		m_y=3;
	}
	virtual DoSomething() const
	{
		cout<<"Man's bomb++"<<endl;
	}
};

class Shoe : public Prop
{
public:
	Shoe()
	{
		m_x=4;
		m_y=4;
	}
	virtual DoSomething() const
	{
		cout<<"Man's Speed++"<<endl;
	}
};

class Liquid : public Prop
{
public:
	Liquid()
	{
		m_x=5;
		m_y=5;
	}
	virtual DoSomething() const
	{
		cout<<"Blast's Power++"<<endl;
	}
};

int main()
{
	Anima test;
//	Prop test1;
	vector<Prop*> PropVec;
	srand( (unsigned)time( NULL ) );
	int loop=0;

	//产生道具对象；
	while(loop<20)
	{
		switch( rand()%6 )
		{
		case 0:
		case 1:
		case 2:
			break;
		case 3:
			cout<<"Bomb"<<endl;
			PropVec.push_back(new Bomb);
			break;
		case 4:
			cout<<"Shoe"<<endl;
			PropVec.push_back(new Shoe);
			break;
		case 5:
			cout<<"Liquid"<<endl;
			PropVec.push_back(new Liquid);
			break;
		}
		loop++;
	}
	//画道具；
	for (int i=0; i<PropVec.size(); i++)
	{
		cout<<"--------------------------------------"<<endl;
		PropVec[i]->Draw();
		PropVec[i]->DoSomething();
		cout<<"--------------------------------------"<<endl;
	}

	//判断道具是否被吃；
	for (vector<Prop*>::iterator Iter=PropVec.begin(); Iter!=PropVec.end(); Iter++)
	{
		if( (*Iter)->IsEatByMan(rand()%3) )	
		{
			delete *Iter;
			(*Iter)=NULL;
		}
		//(*Iter)->IsEatByMan(rand()%3);
	}

	//处理调对象集；
	for (i=0; i<PropVec.size(); i++)
	{
		if(PropVec[i])
			delete PropVec[i];
	}
	PropVec.clear();
	return 0;
}
