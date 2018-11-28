/*************************************************************************/
/* bparser - a bison-based, C99 parser                                   */
/* Copyright (C) 2015-2018                                               */
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

#include <iosfwd>
#include <vector>
#include <stdexcept>

#include "node_fwd.h"

template<class PtrType>
class decl_auto_ptr
{
	PtrType* ptr;
	static void assert_p(const PtrType* p) {
		if(!p) throw std::logic_error("pointer access");
	}

public:
	decl_auto_ptr(const decl_auto_ptr& other) : ptr(NULL) { if(other.ptr) throw std::logic_error("copy ctor requires NULL"); } // forbidden

	decl_auto_ptr() : ptr(NULL) {}
	~decl_auto_ptr() { clear(); }
	
	PtrType* alloc() { if(ptr) throw std::logic_error("overwriting pointer"); else return ptr = new PtrType; }
	void clear() { delete ptr; ptr = NULL; }

//	operator PtrType*() { return ptr; }
	operator PtrType const*() const { return ptr; }
	operator bool() const { return !!ptr; }
	PtrType* raw() { return ptr; }
	const PtrType* raw() const { return ptr; }
	PtrType* steal() { PtrType* res = ptr; ptr = NULL; return res; }
	PtrType const* steal() const { PtrType res = ptr; ptr = NULL; return res; }
	PtrType& operator*() { assert_p(ptr); return *ptr; }
	PtrType const& operator*() const { assert_p(ptr); return *ptr; }
	PtrType* operator->() { assert_p(ptr); return ptr; }
	PtrType const* operator->() const { assert_p(ptr); return ptr; }
	bool operator!() const { return ptr == NULL; }
	
	void assign(PtrType* target) { ptr = target; }
	void take_ownership(decl_auto_ptr<PtrType>& other)
	{
		ptr = other.ptr;
		other.ptr = NULL;
	}
};

class not_copyable
{
	not_copyable(const not_copyable& ) {}
public:
	not_copyable() {}
};

struct has_type_qualifiers
{
	struct type_qualifiers_t {
		unsigned is_const : 1;
		unsigned is_restrict : 1;
		unsigned is_volatile : 1;
		unsigned is_atomic : 1;
		type_qualifiers_t() : is_const(0), is_restrict(0), is_volatile(0), is_atomic(0) {}
		std::string to_str() const;
		std::string to_human() const { return to_str(); }
	} type_qualifiers;
};

//! class representing all declaration specifier info of a declaration
struct declspec_info_t : public has_type_qualifiers, not_copyable
{
	struct function_specifiers_t {
		unsigned is_inline : 1;
		unsigned is_noreturn : 1;
		function_specifiers_t() : is_inline(0), is_noreturn(0) {}
	} function_specifiers;
	struct storage_class_t {
		//! the type is resolved, this is only a hint that this type
		//! is being declared as a typedef
		unsigned is_typedef : 1;
		unsigned is_extern : 1;
		unsigned is_static : 1;
		unsigned is_thread_local : 1;
		unsigned is_register : 1;
		storage_class_t() : is_typedef(0), is_extern(0), is_static(0), is_thread_local(0), is_register(0) {}
	} storage_class;
	struct type_specifiers_t
	{
		enum base_type_t
		{
			is_void,
			is_bool,
			is_complex,
			is_char,
			is_int,
			is_float,
			is_double,
			is_enum_spec,
			is_struct_or_union
		} base_type;
		enum length_mod_t
		{
			no_length_mod,
			is_short,
			is_long,
			is_long_long
		} length_mod;
		bool is_signed; // only for char
		bool is_unsigned;
		bool is_union;
		nodes::enum_specifier_t* enum_spec;
		nodes::struct_or_union_specifier_t* struct_or_union_spec;
		type_specifiers_t() :
			base_type(is_void),
			length_mod(no_length_mod), is_signed(false), is_unsigned(false), is_union(false),
			enum_spec(NULL),
			struct_or_union_spec(NULL)
		{
		}
	} type_specifiers;

	std::string to_str() const;
	std::string to_human() const { return to_str(); }

	void cleanup();
};

//! class representing stuff like 'const volatile **'
struct pointer_info_t : public has_type_qualifiers, not_copyable
{
	unsigned pointers; //!< number of pointer symbols until next pointer info
	decl_auto_ptr<struct pointer_info_t> next;
	pointer_info_t() : pointers(0) {}

	std::string to_str() const;
	std::string to_human() const;
};

// entry point
struct declarator_info_t : not_copyable
{
	bool is_func;
	bool is_decl;
	bool is_array; // exact array size is not specified currently
	bool is_array_without_size;
	std::vector< decl_auto_ptr<struct declarator_info_t> > params;
	nodes::identifier_t* identifier;
	bool is_idlist;
	bool is_ellipsis;
	
	decl_auto_ptr<declspec_info_t> declspec;
	decl_auto_ptr<pointer_info_t> pointers;
	decl_auto_ptr<struct declarator_info_t> next;
	
	declarator_info_t() :
		is_func(false),
		is_decl(false),
		is_array(false),
		is_array_without_size(false),
		identifier(NULL),
		is_idlist(false),
		is_ellipsis(false) {}

	std::string to_str(const std::string& inside = "") const;
	std::string to_human() const;
	declspec_info_t* is_declspec_only() const
	{
		if(is_func || is_decl || is_array || is_array_without_size || is_idlist || is_ellipsis)
		{
			return NULL;
		}
		else
		{
		//	declspec_info_t* dir = 
		}
	}
};

std::ostream& operator<<(std::ostream& os, const declarator_info_t& dsi);

declarator_info_t* get_declspec_info(nodes::defined_t& node);


