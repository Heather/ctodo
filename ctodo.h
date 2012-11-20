/*          ctodo - Light TODO library
          Copyright (C)  2012  Ash Harley

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public
License as published by the Free Software Foundation; either
version 3.0 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA*/

#ifdef WIN32
#include <Windows.h>
#endif

extern void initdb();
extern void version();
extern void help(char* argv);
extern int set(char** argv, int argc);
extern int sync(char** argv);
extern void edit(char** argv, int argc);
extern void swap(char** argv);
extern void mv(char** argv);
extern void clean();
extern void rm(char** argv);
extern int read(char** argv, int argc);
extern int write(char** argv, int argc);

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
