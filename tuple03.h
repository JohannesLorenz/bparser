#include <cstddef>

struct no_fill {};

class null_type
{
public:
	typedef null_type type;
	typedef null_type next;
	void fill(no_fill, no_fill, no_fill, no_fill,
		no_fill, no_fill, no_fill, no_fill) {}
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

template<class T, class Next = null_type>
class tpl : public Next
{
	typedef tpl<T, Next> self;
public:
	typedef T type;
	typedef Next next;
	type value;

	tpl() {}
	//tpl(const type& value) : value(value) {}

	// TODO: const ... & -> one new typedef-struc

	template<std::size_t Idx>
	struct ar {
		typedef const typename type_at<self, Idx>::type& g;
		typedef typename type_at<self, Idx>::type c;
	};

	tpl(const type& e1,
		typename ar<1>::g e2 = typename ar<1>::c(),
		typename ar<2>::g e3 = typename ar<2>::c(),
		typename ar<3>::g e4 = typename ar<3>::c(),
		typename ar<4>::g e5 = typename ar<4>::c(),
		typename ar<5>::g e6 = typename ar<5>::c(),
		typename ar<6>::g e7 = typename ar<6>::c(),
		typename ar<7>::g e8 = typename ar<7>::c()
		) :
		Next(e2, e3, e4, e5, e6, e7, e8, null_type()),
		value(e1)
	{
	}


	Next& set(const type& _value) { return value = _value, *this; }

#if 0
	template<class T2> void fill(const T& e1, const T2& e2)
		{ set(e1).fill(e2); }

	template<class T2, class T3>
	void fill(const T& e1, const T2& e2, const T3& e3) {
		set(e1).fill(e2, e3);
	}

	template<class T2, class T3, class T4>
	void fill(const T& e1, const T2& e2, const T3& e3, const T4& e4) {
		set(e1).fill(e2, e3, e4);
	}

	template<class T2, class T3, class T4, class T5>
	void fill(const T& e1, const T2& e2, const T3& e3, const T4& e4,
		const T5& e5) {
		set(e1).fill(e2, e3, e4, e5);
	}

	template<class T2, class T3, class T4,
		class T5, class T6>
	void fill(const T& e1, const T2& e2, const T3& e3, const T4& e4,
		const T5& e5, const T6& e6) {
		set(e1).fill(e2, e3, e4, e5, e6);
	}

	template<class T2, class T3, class T4,
		class T5, class T6, class T7>
	void fill(const T& e1, const T2& e2, const T3& e3, const T4& e4,
		const T5& e5, const T6& e6, const T7& e7) {
		set(e1).fill(e2, e3, e4, e5, e6, e7);
	}

	template<class T2, class T3, class T4,
		class T5, class T6, class T7, class T8>
	void fill(const T& e1, const T2& e2, const T3& e3, const T4& e4,
		const T5& e5, const T6& e6, const T7& e7, const T8& e8) {
		set(e1).fill(e2, e3, e4, e5, e6, e7, e8);
	}
#endif
};

template<class T>
class tpl<T, null_type>
{
public:
	typedef T type;
	typedef null_type next;
	type value;

	tpl() {}

	tpl(const type& value, const null_type&, const null_type&,
		const null_type&, const null_type&, const null_type&,
		const null_type&, const null_type&) :
		value(value) {}

	void set(const type& _value) { value = _value; }

	template<class T1>
	void fill(const T1& e1) { set(e1); }
};

template<class T, class Next = null_type>
class pt : public tpl<T*, Next>
{
	typedef pt<T, Next> self;
public:
	pt() {}
//	pt(T* value) : tpl<T*, Next>(value) {}

	template<std::size_t Idx>
	struct ar {
		//typedef const typename type_at<self, Idx>::type& g;
		typedef typename type_at<self, Idx>::type c;
		typedef c g;
	};

	pt(T* const& e1,
		typename ar<1>::g e2 = typename ar<1>::c(),
		typename ar<2>::g e3 = typename ar<2>::c(),
		typename ar<3>::g e4 = typename ar<3>::c(),
		typename ar<4>::g e5 = typename ar<4>::c(),
		typename ar<5>::g e6 = typename ar<5>::c(),
		typename ar<6>::g e7 = typename ar<6>::c(),
		typename ar<7>::g e8 = typename ar<7>::c()
		) :
		tpl<T*, Next>(e1, e2, e3, e4, e5, e6, e7, e8)
	{
	}
};

template<class T, class Next = null_type>
class ptn : public pt<T, Next>
{
	typedef pt<T, Next> base;
	typedef ptn<T, Next> self;
public:
	template<std::size_t Idx>
	struct ar {
		//typedef const typename type_at<self, Idx>::type& g;
		typedef typename type_at<self, Idx>::type c;
		typedef c g;
	};

	ptn() : base(NULL) {}
	ptn(T* value) : base(value) {}

	ptn(T* const& e1,
		typename ar<1>::g e2 = typename ar<1>::c(),
		typename ar<2>::g e3 = typename ar<2>::c(),
		typename ar<3>::g e4 = typename ar<3>::c(),
		typename ar<4>::g e5 = typename ar<4>::c(),
		typename ar<5>::g e6 = typename ar<5>::c(),
		typename ar<6>::g e7 = typename ar<6>::c(),
		typename ar<7>::g e8 = typename ar<7>::c()
		) :
		pt<T, Next>(e1, e2, e3, e4, e5, e6, e7, e8)
	{
	}
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


