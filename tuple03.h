#include <cstddef>

class null_type {};

template<class T, class Next>
class tpl : public Next
{
public:
	typedef T type;
	typedef Next next;
	type value;
};

template<class T>
class tpl<T, null_type>
{
public:
	typedef T type;
	type value;
};

template<class Tpl, std::size_t Idx>
struct type_at
{
	typedef typename type_at<typename Tpl::next, Idx - 1>::type type;
};

template<class Tpl>
struct type_at<Tpl, 0>
{
	typedef typename Tpl::type type;
};

template<class Tpl, std::size_t Idx>
struct _value_at
{
	static typename type_at<Tpl, Idx>::type& get(Tpl& tpl) { return _value_at<typename Tpl::next, Idx-1>::get(tpl); }
};

template<class Tpl>
struct _value_at<Tpl, 0>
{
	static typename type_at<Tpl, 0>::type& get(Tpl& tpl) { return tpl.value; }
};

template<std::size_t Idx, class Tpl>
typename type_at<Tpl, Idx>::type& value_at(Tpl& tpl) { return _value_at<Tpl, Idx>::get(tpl); }


