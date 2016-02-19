#include "geom.h"
#include <vector>
#include <string>
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

std::ostream& span_t::print(bool full_paths, std::ostream& stream) const
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
	
	if(s.second.line != 0) {
		/*if(s.first.line == s.second.line)
			stream << ", l" << s.first.line << ", c" << s.first.col << "-" << s.second.col;
		else
			stream	<< ", l" << s.first.line << ", c" << s.first.col << "-"
				<< "l" << s.second.line << ", c" << s.second.col;*/
		stream << ": l" << s.first.line << ", c" << s.first.col << "-";
		if(s.first.line != s.second.line)
			stream << "l" << s.second.line << ", c";
		stream << s.second.col;
	}
	return stream;
}

std::ostream& operator<<(std::ostream& stream, const span_t& s) {
	return s.print(true, stream);
}

