#include "geom.h"
#include <vector>
#include <string>
#include <iostream>

extern std::vector<std::string>& get_files();

std::ostream& operator<<(std::ostream& stream,
	const geom_t& g)
{
	return stream << "geometry(line: " << g.line << ", col: " << g.col << ")";
}

std::ostream& operator<<(std::ostream& stream,
	const span_t& s)
{
	return stream << get_files().at(s.first.file_id) << ": "
		<< "l" << s.first.line << ", c" << s.first.col;
}
