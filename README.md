TODO List management util: Usage & Version
==========================================

```c
       if(strcmp(argv[1], "--version") == 0) {
            printf("v0.01\n");
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

Synchronization
===============

Easy, seens everyone here have got his ~ in git all you need is just
git add .todo.db3 so you sync you todo list in binary way with .git
Yes encrytion doesn't supported but I don't need it because I will be
happy if you take some task from my todo list and make it for me ;)

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
git clone git@github.com:nCdy/todo.git; cd todo
make
make install
```

License
=======

GPL v3
------
