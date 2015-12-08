enum e1
{
	first = 0 + 1,
	second,
};

enum e2
{
	zero = first
};

int main()
{
	enum e3 { e3_first, } e3_variable;
	enum e4 { e4_first } e4_variable;

	sizeof(zero);
	sizeof(first);
	sizeof(second);
	sizeof(e3_first);
	sizeof(e4_first);
	sizeof(e3_variable);
	sizeof(e4_variable);
}
