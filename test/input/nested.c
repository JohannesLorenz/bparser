typedef struct { int x; } s;

int (*f)(int (*g)(void));

s (*(*(*f3)(s s1))(s s1))(s s1);

int main() {
	int x, g, s1;
	sizeof(s);
	sizeof(f);
	sizeof(f3);
}
