typedef union {
	int x;
} u;

struct b {
	const char;
	int x, :15,  rest:1;
};

struct c { float f; } ;
typedef struct { float f; } d;

int main()
{
	struct b b_variable;
	u u_variable;
}
