TODO List management util: Usage & Version
==========================================

 - example usage on windows

![source-code](https://raw.github.com/nCdy/ctodo/master/ss/w1.JPG)

 - example on linux + synchronization

![source-code](https://raw.github.com/nCdy/ctodo/master/ss/nix1.JPG)

 - sync changes to windows

![source-code](https://raw.github.com/nCdy/ctodo/master/ss/w2.JPG)

 - see changes

![source-code](https://raw.github.com/nCdy/ctodo/master/ss/w3.JPG)

 - work from web also possible via GitHub \o/

![source-code](https://raw.github.com/nCdy/ctodo/master/ss/h1.JPG)

Have fun...

Installation
============

For Unix-like
---------
```bash
git clone git@github.com:nCdy/todo.git; cd todo
make
make install
```

For Gentoo
----------
```bash
layman -a nengraphy
emerge todo
```

For Windows
----------
Compilable with MSVC 2010, there is project file

Binaries
----------
Available on codeplex : http://ctodo.codeplex.com/

License
-------

The GNU General Public License v3.0

```c
        if (strcmp(argv[1], "--version") == 0) {
            printf("v1.01\n\r");
            return 0;
            }
        if (strcmp(argv[1], "initdb") == 0
                || strcmp(argv[1], "read") == 0
                || strcmp(argv[1], "r") == 0
                || strcmp(argv[1], "edit") == 0
                || strcmp(argv[1], "e") == 0
                || strcmp(argv[1], "clean") == 0
                || strcmp(argv[1], "rm") == 0
                || strcmp(argv[1], "write") == 0
                || strcmp(argv[1], "w") == 0
                || strcmp(argv[1], "set") == 0
                || strcmp(argv[1], "swap") == 0
                || strcmp(argv[1], "sync") == 0) {
```