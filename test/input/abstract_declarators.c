void f(int (*)[*]);
void g(int[]);
void h(int[1+2]);
void i(int [*]);
void j(int *[]);
void k(int [1+2][3+4]);
void l(int *());
void m(int ());
void n(int (*const [])(unsigned int x, char*, ...));
void o(int (int));
void p(int **());
void q(int * const*());

int main()
{
	sizeof(const char*);
}