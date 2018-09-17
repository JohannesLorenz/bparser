inline void f() {}
extern void g();
void h() __attribute__((packed, weak));
int main()
{
	typedef int my_int;
	static signed short a;
	register unsigned char b;
	auto long c;
	double d;
	_Bool e;
	my_int g;
	struct { _Complex f; } s;
	enum { first } h;
	int f;
}
