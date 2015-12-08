void func(int a[],
	int b[*],
	int c[static const 2*2],
	int d[const static 2*2],
	int e[const restrict*],
	int f[const 2*2],
	int g[const],
	int h[3*4],
	int (*j)(),
	int (*k)(int, const char* const s)
);

void func2(x, y) int x, y; { sizeof(x); }

int main()
{
	int a, b, c, d, e, f, g, h, j, k, x, y;
	func2(a);
}
