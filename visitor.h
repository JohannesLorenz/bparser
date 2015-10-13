#include <cstddef>
#include <iosfwd>

class visitor_t
{
public:
	virtual void visit(number_t *e) = 0;
	virtual void visit(expression_t *e) = 0;
	virtual ~visitor_t() {}
};

namespace std
{
	extern ostream cout;
}

class dumper_t : public visitor_t
{
	std::size_t depth;
	std::ostream& stream;
	void handle_depth();
public:
	dumper_t(std::ostream& stream = std::cout) : depth(0), stream(stream) {};
	void visit(number_t *e);
	void visit(expression_t *e);
};

struct cleaner_t : visitor_t
{
	void visit(number_t *e) { delete e; }
	void visit(expression_t *e);
};

//node_t* echo(node_t* e);
