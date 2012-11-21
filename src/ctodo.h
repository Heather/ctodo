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
//________________________________________________________________________________
#ifdef WIN32
#include <Windows.h>
#endif
//________________________________________________________________________________
extern void todo_initdb();
extern char* todo_version();
extern char* todo_help();
extern int todo_set(char** argv, int argc);
extern int todo_sync(char** argv);
extern void todo_edit(char** argv, int argc);
extern void todo_swap(char** argv);
extern void todo_mv(char** argv);
extern void todo_clean();
extern void todo_rm(char** argv);
extern int todo_read(char** argv, int argc);
extern int todo_write(char** argv, int argc);
//________________________________________________________________________________
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
