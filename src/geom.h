//! @file geom.h Layout classes for AST nodes
//!	geometry means: which lines, columns and files are the nodes in

#ifndef GEOM_H
#define GEOM_H

#include <iosfwd>

//! This class contains a specific position in a file
struct geom_t
{
	int file_id;
	int line;
	int col, tabcol;
	geom_t(int file_id, int line, int col, int tabcol) :
		file_id(file_id), line(line), col(col), tabcol(tabcol) {}
	geom_t() : file_id(-1), line(-1), col(-1), tabcol(-1) {}

	const std::string& file() const;
	friend std::ostream& operator<<(std::ostream& stream, const geom_t&);
};

inline bool operator<(const geom_t& g1, const geom_t& g2) {
	return (g1.line == g2.line)
			? g1.col < g2.col
			: g1.line < g2.line;
}

std::string to_string(const geom_t& geom);

//! This class contains a starting and ending position for a node
struct span_t
{
	geom_t first, second;
	span_t(int file_id, int line, int col, std::size_t width,
		int tabcol, std::size_t tabwidth) :
		first(file_id, line, col, tabcol),
		second(file_id, line, col + width, tabcol + tabwidth)
	{
	}
	span_t(geom_t first, geom_t second) : first(first), second(second) {}
	span_t() {}

	static span_t null_span() { return span_t(0,0,0,0,0,0); }

	std::ostream& print(bool full_paths, std::ostream& stream) const;
	friend std::ostream& operator<<(std::ostream& stream, const span_t&);
};

#endif // GEOM_H
