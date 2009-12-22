#include <vector>
#include <iostream>
#include <functional>
#include <iterator>
#include <algorithm>

void doSomething(int i, const char *c);


// 我的第一个二元功能函数，
// 首先，我假定doSomething是某个库函数，
// 我并没有它的源代码。

// 关于可移植性：MS VC6.0不喜欢在模板的返回类型中使用void，
// 所以在MS VC6.0中对operator( )稍作修改，使它返回一个类型（如true）
struct doSomethingWrapper : public 
	std::binary_function<int, const char *, void>
{
	void operator()(int iValue, const char *cValue) const
	{
		doSomething(iValue, cValue);
	}
};

// 现在，就建立了一个内部的功能函数。
// 关于可移植性，同上。
struct doSomethingDirect : public 
	std::binary_function<int, const char *, void>
{
	void operator()(int iValue, const char *cValue) const
	{
		std::cout << cValue 
			<< " " 
			<< iValue 
			<< ". " ;
	}
};

// 这是个帮助器模板，因为我比较懒，它能减少打字量。
template <class Collection, class Function>
Function for_all(Collection &c, const Function &f) 
{
	return std::for_each(c.begin(), c.end(), f);
}

int main()
{
	// 首先，建立vector。

	std::vector<int> vect;
	for (int i=0; i<10; ++i) {
		vect.push_back(i);
	}

	for_all(vect, std::bind2nd(doSomethingWrapper(), "Wrapper:"));

	std::cout << "\n";

	for_all(vect, std::bind2nd(doSomethingDirect(), "Direct:"));

	return 0;
}

// 我独树一帜的第三方库函数
void doSomething(int i, const char *c)
{
	std::cout << c << " " << i << ". " ;
}
