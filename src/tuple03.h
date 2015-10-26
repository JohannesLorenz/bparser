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

// FEATURE: allow last type to be not of null_type, but custom? e.g.
// ptn< a, ptn < b, c> > instead of ptn< a, ptn <b, ptn < c > > >

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
	static typename type_at<Tpl, Idx>::type& get(Tpl& tpl) { return _value_at<typename Tpl::next, Idx-1>::get(tpl.get_next()); }
};

template<class Tpl>
struct _value_at<Tpl, 0>
{
	static typename type_at<Tpl, 0>::type& get(Tpl& tpl) { return tpl.value; }
};

template<std::size_t Idx, class Tpl>
typename type_at<Tpl, Idx>::type& value_at(Tpl& tpl) { return _value_at<Tpl, Idx>::get(tpl); }

// FEATURE: const versions for both args
template<class Tpl>
struct _foreach {
	template<class F>
	void exec(Tpl& tpl, F& f) {
		f(tpl.value), foreach(tpl.get_next(), f);
	}
};

template<>
struct _foreach<null_type> {
	template<class F>
	void exec(const null_type& , F&) {}
};

template<class Tpl, class F>
void foreach(Tpl& tpl, F& f) { _foreach<Tpl>::exec(tpl, f); }

// FEATURE: inherit from Next, if it's possible and makes sense
template<class T, class Next = null_type>
class tpl // : private Next
{
	template<class Tpl, std::size_t Idx>
	friend struct _value_at;

	typedef tpl<T, Next> self;
public:
	typedef T type;
	typedef Next next; // FEATURE: -> next_t
	Next _next;
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
		/*Next*/_next(e2, e3, e4, e5, e6, e7, e8, null_type()),
		value(e1)
	{
	}

	void fill(const type& e1,
		typename ar<1>::g e2 = typename ar<1>::c(),
		typename ar<2>::g e3 = typename ar<2>::c(),
		typename ar<3>::g e4 = typename ar<3>::c(),
		typename ar<4>::g e5 = typename ar<4>::c(),
		typename ar<5>::g e6 = typename ar<5>::c(),
		typename ar<6>::g e7 = typename ar<6>::c(),
		typename ar<7>::g e8 = typename ar<7>::c()
		) {
		value = e1;
		get_next().fill(e2, e3, e4, e5, e6, e7, e8, null_type());
	}

	Next& get_next() { return /* *this;*/ _next; } // FEATURE: -> next()
	const Next& get_next() const { return /* *this; */ _next; }

	Next& set(const type& _value) { return value = _value, get_next(); }
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

	void fill(const type& _value, const null_type&, const null_type&,
		const null_type&, const null_type&, const null_type&,
		const null_type&, const null_type&)
	{
		value = _value;
	}
	
//	template<class T1>
//	void fill(const T1& e1) { set(e1); }
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


