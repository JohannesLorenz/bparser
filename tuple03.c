#include <vector>
#include <cassert>

#include "tuple03.h"

int main()
{
	tpl< int, tpl < std::vector<int>, null_type > > t;
	std::vector<int> v;
	v.push_back(42);
	value_at<0>(t) = 42;
	value_at<1>(t) = v;

	assert(value_at<0>(t) == 42);
	assert(value_at<1>(t).size() == 1);
	
	return 0;
}