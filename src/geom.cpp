#include "geom.h"
#include <vector>
#include <string>
#include <cstdio>
#include <iostream>

extern std::vector<std::string>& get_files();

std::ostream& operator<<(std::ostream& stream,
	const geom_t& g)
{
	return stream << get_files().at(g.file_id) << ": "
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

std::ostream& operator<<(std::ostream& stream,
	const span_t& s)
{
	stream << get_files().at(s.first.file_id) << ": ";
	if(s.first.line == s.second.line)
		stream << "l" << s.first.line << ", c" << s.first.col << "-" << s.second.col;
	else
		stream	<< "l" << s.first.line << ", c" << s.first.col << "-"
			<< "l" << s.second.line << ", c" << s.second.col;
	return stream;
}
