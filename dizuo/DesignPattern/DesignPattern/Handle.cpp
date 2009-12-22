#include <iostream> 
using namespace std; 


//泛型句柄类 
template <class T> 
class Handle{ 
public: 
	//若p为0，则为未绑定的句柄 
	//应该是用p初始化ptr
	explicit Handle():ptr(0),use(new size_t(0)){} 
	explicit Handle(T* p):ptr(p),use(new size_t(1)){} 

	//支持指针行为的重载操作 
	T& operator*(); 
	T* operator->(); 
	const T& operator*() const; 
	const T* operator->() const; 

	//复制控制：提供常规指针行为，但最后一个句柄删除基础对象 
	explicit Handle(const Handle& h):ptr(h.ptr),use(h.use){ 
		++*use; 
		//---------------------------------
		rem_ref(); //是否需要？？？？？？
		//---------------------------------
	} 
	Handle& operator=(const Handle&); 
	~Handle(){ 
		rem_ref(); 
	} 
private: 
	T* ptr;            //共享的对象 
	size_t *use;        //使用计数：多少个句柄指向*ptr 
	void rem_ref() 
	{ 
		if(--*use==0){ 
			delete ptr; 
			delete use; 
		} 
	} 
}; 
template <class T> 
inline Handle <T>& Handle <T>::operator =(const Handle &rhs){ 
	++*rhs.use;        //防止自身赋值 
	rem_ref();          //使用计数减1并在必要时删除指针 
	ptr=rhs.ptr; 
	use=rhs.use; 
	return *this; 
} 
template <class T> 
inline T& Handle <T>::operator*(){ 
	if(ptr) return *ptr; 
	else 
		throw std::runtime_error("dereference of unbound Handle"); 
} 
template <class T> 
inline T* Handle <T>::operator->() 
{ 
	if(ptr) return ptr; 
	else 
		throw std::runtime_error("access through unbound Handle*"); 
} 
template <class T> 
inline const T& Handle <T>::operator* () const 
{ 
	if(ptr) return *ptr; 
	else 
		throw std::runtime_error("dereference of unbound Handle"); 
} 
template <class T> 
inline const T* Handle <T>::operator-> () const 
{ 
	if(ptr) return ptr; 
	else 
		throw std::runtime_error("access through unbound Handle*"); 
} 


//cpp文件 
int main()
{ 
	Handle <int> hp(new int(42)); 

// 		Handle <int> hp2;	申明
// 		hp2 = hp;			定义
		Handle<int> hp2(hp);
		cout << *hp2 << " " << *hp2 <<endl; 
		*hp2 = 10; 

	cout << *hp <<endl; 
	return 0; 
} 

//////////////////////////////////////////////////////////////////////////
//  [4/7/2009 RenYaFei]
//	模板内部size_t的使用
//  变量申明 定义的区别
//	const使用
//	传引用的效率
//	explicit的使用好处