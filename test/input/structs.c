typedef union {
	int x;
} u;

struct b {
	const char;
	int x, :15,  rest:1;
};

int main()
{
	struct b b_variable;
	u u_variable;
}