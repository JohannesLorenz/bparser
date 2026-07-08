typedef void * va_list;
typedef unsigned int Stgzustbit;

void f(va_list *ap)
{
	Stgzustbit x = __builtin_va_arg(*ap, const Stgzustbit);
	int y = __builtin_va_arg(*ap, int);
}