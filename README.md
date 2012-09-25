TODO List management util: Usage & Version
==========================================

```shell
>>todo w better ARMD ARMC data transfer                                                           ~/
>>todo r                                                                                          ~/
+--------------------------------------+
| 1 >> TWI interface correction        |
| 2 >> Hangul for NJ                   |
| 3 >> better ARMD ARMC data transfer  |
+--------------------------------------+
>>todo sync                                                                                       ~/
Sync file: .todo.sync
>>git add .todo.sync                                                                              ~/
>>git commit -am "run todo sync to sync from another machine"      
```

```c
        if (strcmp(argv[1], "--version") == 0) {
            printf("v0.01\n\r");
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
                || strcmp(argv[1], "sync") == 0) {
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
git clone git@github.com:nCdy/todo.git; cd todo
make
make install
```
not that you need to have sqlite3 installed for this way

License
-------

The GNU General Public License v3.0
