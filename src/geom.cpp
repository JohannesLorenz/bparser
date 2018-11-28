/*************************************************************************/
/* bparser - a bison-based, C99 parser                                   */
/* Copyright (C) 2015-2016                                               */
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

#include "geom.h"
#include <vector>
#include <cstring>
#include <string>
#include <cstdio>
#include <iostream>

extern std::vector<std::string>& get_files();

const std::string& geom_t::file() const {
	return get_files().at(file_id);
}

std::ostream& operator<<(std::ostream& stream,
	const geom_t& g)
{
	return stream << g.file() << ": "
		<< "l" << g.line << ", c" << g.col;
}

std::string to_string(const geom_t& g)
{
	char tmp[16];
	std::string result = get_files().at(g.file_id) + ": l";
	sprintf(tmp, "%d", g.line);
	result.append(tmp).append(", c");
	sprintf(tmp, "%d", g.col);
	return result += tmp;
}

std::ostream& span_t::print(bool full_paths, std::ostream& stream, bool long_numbers) const
{
	const span_t& s = *this;
	if(full_paths)
		stream << get_files().at(s.first.file_id);
	else
	{
		const char* ptr1 = get_files().at(s.first.file_id).c_str();
		const char* ptr2 = strrchr(ptr1, '/');
		stream << (ptr2 ? (ptr2+1) : ptr1);
	}

	const char* line_word = long_numbers ? "line " : "l";
	const char* col_word = long_numbers ? "col " : "c";
	const char* cols_word = long_numbers ? "columns " : "c";
	const char* to_word = long_numbers ? " -> " : "-";
	
	if(s.second.line != 0) {
		/*if(s.first.line == s.second.line)
			stream << ", " << line_word << s.first.line <<
				", " << cols_word << s.first.tabcol << "-" << s.second.tabcol;
		else
			stream	<< ", " << line_word << s.first.line <<
				<< ", " << col_word  << s.first.tabcol << "-"
				<< line_word << s.second.line
				<< ", " << col_word << s.second.tabcol;*/
		stream << ": " << line_word << s.first.line
			<< ", " << col_word << s.first.tabcol << to_word;
		if(s.first.line != s.second.line)
			stream << line_word << s.second.line << ", " << col_word;
		stream << s.second.tabcol;
	}
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const span_t& s) {
	return s.print(true, stream);
}

