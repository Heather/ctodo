TODO List management util: Usage & Version
=======================================================

 - ctodo : general c library
 - cctodo : simple ctodo client in C
 
=======================================================
 
 - good command line interface (use --help for details)
 - easy synchronization via your favorite vcs
 - crossplatform
 - native, library is written in C and Sqlite
 - you can manage your list just from anywhere
 - it could be your personal psychologist, trust me
 - it's fancy even has got CLI color schemas (after 1.0.4 ver)
 - smart application structure: library -> wrapper -> application
 - so you can write you own client implementations

Installation
=======================================================

For Unix-like
---------
```bash
git clone git://github.com/Cynede/ctodo.git
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
Compilable with MinGW to compile run `build.bat`

Also there is Visual Studio stuff here:
	https://github.com/Cynede/wtodo

Compilable with MSVC 2010 / 2012, there is 

 - project files for everything 
 - bat files for fast compilation
 - additional testing in scripts
 - GUI: work in progress (but I'm lazy)

License
-------
The GNU General Public License v2.0
