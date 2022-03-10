/*************************************************************************/
/* bparser - a bison-based, C99 parser                                   */
/* Copyright (C) 2015-2021                                               */
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

#include <cctype>

inline void skip_esc_seq(const char*& p)
{
	if(*p == '\\')
	{
		++p;
		if(*p == '\'' || *p == '"'
			|| *p == '?' || *p == '\\'
			|| *p == 'a' || *p == 'b'
			|| *p == 'f' || *p == 'n'
			|| *p == 'r' || *p == 't'
			|| *p == 'v')
			++p;
		else if(*p > '0' && *p < '7')
		{
			++p;
			if(*p > '0' && *p < '7')
			{
				++p;
				if(*p > '0' && *p < '7')
				{
					++p;
				}
			}
		}
		else if(*p == 'x')
		{
			while(isxdigit(*p)) ++p;
		}
	}
}