typedef union {
	int x;
} u;

struct b {
	const char;
	int x, :15, rest:1;
} __attribute__((packed, weak));

struct c { float f; } ;
typedef struct { float f; } d;

int main()
{
	struct b b_variable;
	u u_variable;
	int x, rest, f;
	sizeof(u);
	sizeof(struct b);
	sizeof(struct c);
	sizeof(d);
	sizeof(b_variable);
	sizeof(u_variable);
}
