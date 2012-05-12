sholy
=====

```c
  else {
	if ( strcmp(argv[1],"--version") == 0 ) {
	  printf("v0.01\n");
	  return 0;
	  }
	else if  ( strcmp(argv[1],"test") == 0 ) {
	  RunTests(1);
	  return 0;
	  }
	else if ( strcmp(argv[1],"initdb") == 0 
			  || strcmp(argv[1],"read") == 0 
			  || strcmp(argv[1],"clean") == 0
			  || strcmp(argv[1],"rm") == 0
			  || strcmp(argv[1],"write") == 0 ) {
```

Encripting TODO with SQLITE AES ECB PolarSSL

[status: in progress]

License: GPL :3
---------------