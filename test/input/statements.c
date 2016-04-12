int main()
{
	{
	}
	
	{
		;
	}
	
	{
		2+2;
	}

	if(1)
		1;
	else
		1;
	
	lbl:	;
	
	switch (1) {
		case 1: return;
		case 2: break;
		case 3: return 0;
		default: goto lbl;
	}
	
	while(1) { continue; }
	do 1; while(1);
	
	/* comment 1 */
	/* comment 2
		... */ int x;
	for(; x; ) ;
	for(; x; ++x) ;
	for(int y; y; ) ;
	for(int y; y; ++y) ;
	for(int y; y; ++y) { x = y; }
}
