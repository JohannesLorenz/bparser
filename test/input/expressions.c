struct s_t { int x; };
	
int f() { return 42; }
int g(int x, int y) { return 42; }
void h(struct s_t s) {}

int main()
{
	int x, y, z;
	
	x=(z, y?1:2);
	x *= z || y;
	x /= z && y;
	x %= z ^ y;
	x += z | y;
	x -= z & y;
	x <<= z < y;
	x >>= z > y++;
	x &= z != y--;
	x |= z == ++y;
	x ^= z <= --y;
	(z<<1) >= (y>>2);
	!(int)(y + (-1) - (+2) * (~3) / 4 % 5);
	sizeof(x);
	sizeof(1+2);
	sizeof(int);
	
	int *p;
	p = &x;
	*p = 3;
	
	int a[3];
	a[0] + f() + g(42, 42);
	
	struct s_t s;
	s.x + (&s)->x;
	h((struct s_t){ .x = 0 });
}
