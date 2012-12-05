TODO List management util: Usage & Version
=======================================================

 - ctodo : general c library
 - cctodo : simple ctodo client in C
 - nwrap : native wrapper for CLR Languages
 - frodo : windows console client on F# for nwrap testing
 - wtodo (not finished yet) : windows gui C# WPF application
 - atodo (not released yet) : android / iphone gui application
 - additional experiment using ctodo lib on various languages

=======================================================
 
 - good command line interface (use --help for details)
 - easy synchronization via your favorite vcs
 - crossplatform
 - native, library is written in C and Sqlite
 - you can manage your list just from anywhere
 - it could be your personal psychologist, trust me
 - it's fancy even has got CLI color schemas (after 1.0.4 ver)
 - smart structure: library -> wrapper -> application
 - so you can write you own implementations

Installation
=======================================================

For Unix-like
---------
```bash
hg clone https://Cynede@bitbucket.org/Cynede/ctodo
cd todo
make
make install
```

For Gentoo
----------
Probably add my overlay and
```bash
emerge todo
```

For Windows
----------
Compilable with MSVC 2010 / 2012, there is 

 - project files for everything 
 - bat files for fast compilation
 - additional testing in scripts

License
-------
The GNU General Public License v2.0
