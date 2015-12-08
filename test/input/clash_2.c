typedef int t;

int main() {
	sizeof(t);
	int t;
	sizeof(t);
	{
		enum { s } t;

		sizeof(t);

		struct t {
			int t : sizeof(t);
			int a[(3,sizeof(t))];
		};

		sizeof(t);
		sizeof(struct t);

		{
			enum { t };
			sizeof(t);
		}

		int a;
	}
}

