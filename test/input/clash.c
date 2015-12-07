enum { t };

int main()
{
	for(int t = 0; t != 10; ++t) ;
	for(int t = 0; t != 10; ++t) {}
	sizeof(t);
	for(int t = 0; t != 10; ++t) {
		sizeof(t);
	}
	sizeof(t);
	typedef int t;
	sizeof(t);
	{
		t t;
		for(int t; 0; ) ;
	}
}

void f(int t) {
	sizeof(t);
}

