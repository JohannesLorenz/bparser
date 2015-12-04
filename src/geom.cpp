#include "geom.h"
#include <vector>
#include <string>
#include <iostream>

extern std::vector<std::string>& get_files();

std::ostream& operator<<(std::ostream& stream,
	const geom_t& g)
{
	return stream << get_files().at(g.file_id) << ": "
		<< "l" << g.line << ", c" << g.col;
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
