typedef struct s_t { int x; } s_t;

void f(s_t* s)
{
	s->x = 42;
}

int main() {}