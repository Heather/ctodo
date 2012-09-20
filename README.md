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

Easy, seens everyone here have got her ~ in git all you need is just
git add .todo.db3 so you sync your todo list in binary way with git.
Yes encrytion doesn't supported yet but I don't need it because I will be
happy if you will take some tasks from my todo list and make them for me ;)

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

The GNU General Public License v3.0
