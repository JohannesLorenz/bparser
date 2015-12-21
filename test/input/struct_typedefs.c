typedef struct s_t { struct { int x; } s2; } s_t;

void f(s_t* s)
{
	s->s2.x = 42;
}

int main() {}