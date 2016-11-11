#include <future>
#include <iostream>
#include <string>

/***how to use a std::async template function and std::future ***/
/***==================================================***/
/*
int to_find_the_answer()
{
	int result = 0;

	for(int i = 0;i < 1000; ++i)
	{
		result += i;
	}

	return result;
}

int do_other_stuff()
{
	int result = 0;
	for(int i = 0;i < 100;i++)
	{
		result += i;
	}
	return result;
}
*/
/***how to use a std::async template function and std::future ***/
/***==================================================***/



/***how to pass value to the std::async function template ***/
/***==================================================***/
struct X
{
	void foo(int i, const std::string &str)
	{
		std::cout << "foo :" << i << " " << str << std::endl;
	}

	std::string bar (const std::string & str)
	{
		std::cout << "bar :" << str << std::endl;

		return str;
	}
};

X x;

auto f1 = std::async(&X::foo, &x,42,"hello"); //where p is &x;
auto f2 = std::async(&X::bar, x,"goodbye");  //call tempx.bar("goodbye")where tempx is a copy of x


struct Y
{
	double operator()(double d)
	{
		std::cout << "Y : " << d << std::endl;
		return d;
	}
};

Y y;
auto f3 = std::async(Y(),3.14);  //call tempy(3.14) where tmpy is move-constructor from Y()
auto f4 = std::async(std::ref(y),2.718);  //call y(2.718)
auto f7 = std::async(y,2.718);  //call tempy(2.718) where tempy is a copy of y
X baz(X& x)
{
	std::cout << "baz : x"  << std::endl;
	return x;
}
auto f5 = std::async(baz,std::ref(x)); //call baz(x);
//auto f8 = std::async(baz,x);   //wrong example , where tempx is a copy of x, while the function param needs a refernce of x

class move_only
{
public:
	move_only()
	{
		std::cout << "move_only constructor" << std::endl;
	}
	move_only(move_only && move)
	{
		std::cout << "move_only move constructor" <<std::endl;

	}
	move_only(const move_only & move) = delete;
	move_only& operator=(move_only &&move)
	{
		std::cout << "move_only move assignment" << std::endl;
		return *this;
	}
	move_only& operator=(const move_only& move)=delete;

	void operator()()
	{
		std::cout << "move_only operator" << std::endl;

	}
};

auto f6 = std::async(move_only());  // call temp() where temp is consturcted from std::move(move_only())





auto f10 = std::async(std::launch::async,Y(),1.2); //run in new thread
auto f11 = std::async(std::launch::deferred,baz,std::ref(x)); // run in wiat() or get()
auto f12 = std::async(std::launch::deferred|std::launch::async,baz,std::ref(x));//Implementation choose
auto f13 = std::async(baz,std::ref(x));//Implementation choose



/***how to pass value to the std::async function template ***/
/***==================================================***/


int main()
{
/***how to use a std::async template function and std::future ***/
/***==================================================***/
 /*
	std::future<int> the_answer = std::async(to_find_the_answer);
	std::cout << "do other stuff :" << do_other_stuff() << std::endl;
	std::cout << "find the answer :" << the_answer.get() << std::endl;
	std::cout << "do other stuff again :" << do_other_stuff() << std::endl;
*/
/***how to use a std::async template function and std::future ***/
/***==================================================***/




	return 0;
}