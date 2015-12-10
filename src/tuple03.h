/*************************************************************************/
/* bparser - a bison-based, C99 parser                                   */
/* Copyright (C) 2015-2015                                               */
/* Johannes Lorenz (jlsf2013 @ sourceforge)                              */
/*                                                                       */
/* This program is free software; you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation; either version 3 of the License, or (at */
/* your option) any later version.                                       */
/* This program is distributed in the hope that it will be useful, but   */
/* WITHOUT ANY WARRANTY; without even the implied warranty of            */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU      */
/* General Public License for more details.                              */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program; if not, write to the Free Software           */
/* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110, USA  */
/*************************************************************************/

#ifndef TUPLE03_H
#define TUPLE03_H

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
struct type_at;

template<class Tpl>
struct type_at<Tpl, 0>;

/*
template<class Tpl, std::size_t Idx>
struct type_at
{
	typedef typename type_at<typename Tpl::next, Idx - 1>::type type;
};

template<class Tpl>
struct type_at<Tpl, 0>
{
	typedef typename Tpl::type type;
};*/

template<int I> class number {};


template<class Tpl, std::size_t Idx>
struct type_at : public type_at<typename Tpl::next, Idx - 1>
{
	//typedef typename type_at<typename Tpl::next, Idx - 1>::type type;
	//number<Idx> func(number<Idx>) { return number<Idx>(); }
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
	static void exec(Tpl& tpl, F& f) {
		f(tpl.value), foreach(tpl.get_next(), f);
	}
};

template<>
struct _foreach<null_type> {
	template<class F>
	static void exec(const null_type& , F&) {}
};

template<class Tpl, class F>
void foreach(Tpl& tpl, F& f) { _foreach<Tpl>::exec(tpl, f); }

class skip{};

template<class T, class Next = null_type>
class tpl;

template<class T>
class tpl<T, null_type>;

// FEATURE: inherit from Next, if it's possible and makes sense
template<class T, class Next>
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

	// FEATURE: const ... & -> one new typedef-struc

	template<std::size_t Idx>
	struct ar {
		typedef const typename type_at<self, Idx>::type& g;
		typedef typename type_at<self, Idx>::type c;
	};

	tpl(const type& e0,
		typename ar<1>::g e1 = typename ar<1>::c(),
		typename ar<2>::g e2 = typename ar<2>::c(),
		typename ar<3>::g e3 = typename ar<3>::c(),
		typename ar<4>::g e4 = typename ar<4>::c(),
		typename ar<5>::g e5 = typename ar<5>::c(),
		typename ar<6>::g e6 = typename ar<6>::c(),
		typename ar<7>::g e7 = typename ar<7>::c(),
		typename ar<8>::g e8 = typename ar<8>::c(),
		typename ar<9>::g e9 = typename ar<9>::c(),
		typename ar<10>::g e10 = typename ar<10>::c(),
		typename ar<11>::g e11 = typename ar<11>::c()
		) :
		/*Next*/_next(e1, e2, e3, e4, e5, e6, e7,
			e8, e9, e10, e11, null_type()),
		value(e0)
	{
	}

	void fill(const type& e0, // FEATURE: rename: e0, e1,...
		typename ar<1>::g e1 = typename ar<1>::c(),
		typename ar<2>::g e2 = typename ar<2>::c(),
		typename ar<3>::g e3 = typename ar<3>::c(),
		typename ar<4>::g e4 = typename ar<4>::c(),
		typename ar<5>::g e5 = typename ar<5>::c(),
		typename ar<6>::g e6 = typename ar<6>::c(),
		typename ar<7>::g e7 = typename ar<7>::c(),
		typename ar<8>::g e8 = typename ar<8>::c(),
		typename ar<9>::g e9 = typename ar<9>::c(),
		typename ar<10>::g e10 = typename ar<10>::c(),
		typename ar<11>::g e11 = typename ar<11>::c()
		) {
		value = e0;
		return get_next().fill(e1, e2, e3, e4, e5, e6, e7,
			e8, e9, e10, e11, null_type());
	}

	Next& get_next() { return /* *this;*/ _next; } // FEATURE: -> next()
	const Next& get_next() const { return /* *this; */ _next; }

	Next& set(const type& _value) { return value = _value, get_next(); }
	Next& set(const skip& ) { return get_next(); }

	std::size_t size() const { return get_next().size(); }

	// access:

	template<std::size_t Idx>
	typename type_at<self, Idx>::type& get() { return value_at<Idx, self>(*this); }
};

template<class T>
class tpl<T, null_type>
{
	typedef tpl<T, null_type> self;
public:
	typedef T type;
	typedef null_type next;
	type value;

	tpl() {}

	tpl(const type& value, const null_type&, const null_type&,
		const null_type&, const null_type&, const null_type&,
		const null_type&, const null_type&, const null_type&,
		const null_type&, const null_type&, const null_type&) :
		value(value) {}

	void set(const type& _value) { value = _value; }
	void set(const skip& ) {}

	void fill(const type& _value, const null_type&, const null_type&,
		const null_type&, const null_type&, const null_type&,
		const null_type&, const null_type&, const null_type&,
		const null_type&, const null_type&, const null_type&)
	{
		value = _value;
	}

	std::size_t size() const { return 1; }

	template<std::size_t Idx>
	typename type_at<self, Idx>::type& get() {
		if(Idx > 0) throw "tuple index out of range";
		return value_at<0, self>(*this); }

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

	pt(T* const& e0,
		typename ar<1>::g e1 = typename ar<1>::c(),
		typename ar<2>::g e2 = typename ar<2>::c(),
		typename ar<3>::g e3 = typename ar<3>::c(),
		typename ar<4>::g e4 = typename ar<4>::c(),
		typename ar<5>::g e5 = typename ar<5>::c(),
		typename ar<6>::g e6 = typename ar<6>::c(),
		typename ar<7>::g e7 = typename ar<7>::c(),
		typename ar<8>::g e8 = typename ar<8>::c(),
		typename ar<9>::g e9 = typename ar<9>::c(),
		typename ar<10>::g e10 = typename ar<10>::c(),
		typename ar<11>::g e11 = typename ar<11>::c()
		) :
		tpl<T*, Next>(e0, e1, e2, e3, e4, e5, e6, e7,
			e8, e9, e10, e11)
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
	ptn(T* const& e0,
		typename ar<1>::g e1 = typename ar<1>::c(),
		typename ar<2>::g e2 = typename ar<2>::c(),
		typename ar<3>::g e3 = typename ar<3>::c(),
		typename ar<4>::g e4 = typename ar<4>::c(),
		typename ar<5>::g e5 = typename ar<5>::c(),
		typename ar<6>::g e6 = typename ar<6>::c(),
		typename ar<7>::g e7 = typename ar<7>::c(),
		typename ar<8>::g e8 = typename ar<8>::c(),
		typename ar<9>::g e9 = typename ar<9>::c(),
		typename ar<10>::g e10 = typename ar<10>::c(),
		typename ar<11>::g e11 = typename ar<11>::c()
		) :
		pt<T, Next>(e0, e1, e2, e3, e4, e5, e6, e7,
			e8, e9, e10, e11)
	{
	}
};

#endif // TUPLE03_H
