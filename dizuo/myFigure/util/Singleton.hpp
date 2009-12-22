#ifndef SINGLETON_H
#define SINGLETON_H

template <class T>
class Singleton
{
protected:
	Singleton(){}
	~Singleton(){}
public:
	static T* GetInstance()
	{
		if(m_Singleton == 0)
			m_Singleton = new T;
		return ( (T*)m_Singleton );
	}
	static void FreeInstance()
	{
		if(m_Singleton)
		{
			delete m_Singleton;
			m_Singleton = 0;
		}
	}
protected:
	static T *m_Singleton;
};
template <class T> T *Singleton<T>::m_Singleton = 0;

#endif//SINGLETON_H///:~

//Creator
//Manipulator
//Accessor