typedef struct { int x; } s;

int (*(*_f2)(s s1))(s s1);

int (*f)(int (*g)(void));

int f2(int (*g)(int a, int b[sizeof(a)]));

s (*(*(*f3)(s s1))(s s1))(s s1);

int main() {
	int x, g, s1;
	sizeof(s);
	sizeof(f);
	sizeof(f3);
}
