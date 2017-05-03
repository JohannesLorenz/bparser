typedef enum 
{
	first
} e_t;

int main()
{
	e_t e = first;
	sizeof(first);
	sizeof(e);

	int i=(0) + 0xDEADBEEFu + 0777l + 123ul + 456lu + 78llu + 910ull + 11ll;
	char c = u'1';
	c = U'2';
	c = L'\'';
	unsigned multi = '1234';
	
	float f = 0e+1f + 0E-2F + 0E3l + .0e4L + .0 + 5.e3 + 6. + 0x007.A0p+2;
	const char* simple_str = "Hello World!";
	const char* multi_str = "Hello"
		"World"
		"!";
	const char* complex_str = "\'\"\?\\\a\b\f\n\r\t\b\077\xFF";
	const char* funcname = __FUNCTION__;
}
