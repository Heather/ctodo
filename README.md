TODO List management util: Usage & Version
=======================================================

[![Build Status](https://travis-ci.org/Heather/ctodo.png?branch=master)](https://travis-ci.org/Heather/ctodo)

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

How to Build / Install
=======================================================

For Gentoo
----------
Probably add my overlay and
```bash
emerge todo
```

For Unix-like
---------
```bash
git clone git://github.com/Heather/ctodo.git
cd todo
make
make install
```

For Windows / MinGW
-------------------
run `build.bat` in `./mingw`

For Windows / MSVC
------------------

 - for VS 2012 run `build_Release.bat` in `./msvc`
 - for VS 2010 run `build_Release_100.bat` in `./msvc`

License
-------
Public domain
