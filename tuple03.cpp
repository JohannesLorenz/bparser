#include <vector>
#include <cassert>

#include "src/tuple03.h"

int main()
{
	std::vector<int> v;
	v.push_back(42);


	tpl< int, tpl < std::vector<int> > > t(42, v);
	/*value_at<0>(t) = 42;
	value_at<1>(t) = v;*/
	t.set(42).set(v);

	assert(value_at<0>(t) == 42);
	assert(value_at<1>(t).size() == 1);

	ptn< int, ptn < std::vector<int> > > t2;
	assert(value_at<0>(t2) == NULL);
	assert(value_at<1>(t2) == NULL);

	value_at<1>(t2) = &v;

	assert(value_at<1>(t2)->size() == 1);

	tpl< int, tpl< int > > two_ints;
	two_ints.fill(42, 1);
	
	assert(value_at<0>(two_ints) == 42);
	assert(value_at<1>(two_ints) == 1);

	two_ints.set(skip()).set(0);
	assert(value_at<0>(two_ints) == 42);
	assert(value_at<1>(two_ints) == 0);

	return 0;
}
