todo
=====

```c
       if(strcmp(argv[1], "--version") == 0) {
            printf("v0.01\n");
            return 0;
            }
        if(strcmp(argv[1], "--help") == 0) {
            help(argv[0]);
            return 0;
            }
        if(strcmp(argv[1], "initdb") == 0
                || strcmp(argv[1], "read") == 0
                || strcmp(argv[1], "r") == 0
                || strcmp(argv[1], "clean") == 0
                || strcmp(argv[1], "rm") == 0
                || strcmp(argv[1], "write") == 0
                || strcmp(argv[1], "w") == 0) {
```

Installation
============

For Gentoo
----------
```bash
layman -a nengraphy
emerge todo
```

Other
---------
```bash
git clone git@github.com:Sholy/todo.git; cd todo
make install
```