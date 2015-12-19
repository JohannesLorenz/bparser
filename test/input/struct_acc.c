typedef struct a_t
{
	int b;
} a_t_1;

typedef a_t_1 a_t_2[1];
a_t_2 a;

int main()
{
	int b;
	(*(1 + (&a[0]))).b;
}
