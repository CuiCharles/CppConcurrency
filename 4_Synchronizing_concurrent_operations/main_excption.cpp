#include <iostream>
#include <stdexcept>
#include <cmath>
#include <future>

double square_root(double x)
{
	if(x < 0)
	{
		throw std::out_of_range("x < 0");
	}

	return std::sqrt(x);
}


int main()
{

	//double a = square_root(-1);
	std::future<double> f = std::async(square_root,-1);
	f.get();

	return 0;
}