/*          todo - Light TODO list
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

#include "ctodo.h"
#include <stdio.h>
#include <time.h>
#include <string.h>

int main(int argc, char* argv[]) {
    /*HINSTANCE LoadMe;
    LoadMe = LoadLibrary(L"ctodo_lib_100.dll");
    if (LoadMe != 0)
        printf("todo library loaded!\n");
    else
        printf("todo library failed to load!\n");*/
    if (argc < 2) {
        help(argv[0]);
        return 0;
        }
    else {
        if (strcmp(argv[1], "--version") == 0) {
            version();
            return 0;
            }
        else if (strcmp(argv[1], "--help") == 0) {
            version();
            help(argv[0]);
            return 0;
            }
        else  {
            if (strcmp(argv[1], "initdb") == 0) {
                initdb();
                return 0;
                }
            if (strcmp(argv[1], "set") == 0) {
                if (argc < 4) {
                    printf("set what?\n\r");
                    return 0;
                    }
                else {
                    return set(argv, argc);
                    }
                }
            if (strcmp(argv[1], "sync") == 0) {
                return sync(argv);
                }
            else if ((strcmp(argv[1], "edit") == 0) || (strcmp(argv[1], "e") == 0)) {
                if (argc < 3) printf("edit what?\n\r");
                else {
                    edit(argv, argc);
                    }
                return 0;
                }
            else if ((strcmp(argv[1], "swap") == 0) || (strcmp(argv[1], "s") == 0)) {
                if (argc < 3) printf("swap what?\n\r");
                else {
                    swap(argv);
                    }
                return 0;
                }
            else if (strcmp(argv[1], "mv") == 0) {
                if (argc < 4) printf("move what? you need to provide two indexes\n\r");
                else {
                    mv(argv);
                    }
                return 0;
                }
            else if (strcmp(argv[1], "clean") == 0) {
                clean();
                return 0;
                }
            else if (strcmp(argv[1], "rm") == 0) {
                if (argc < 3) printf("remove what?\n\r");
                else {
                    rm(argv);
                    }
                return 0;
                }
            else if ((strcmp(argv[1], "read") == 0) || (strcmp(argv[1], "r") == 0)) {
                return read(argv, argc);
                }
            else {
                return write(argv, argc);
                }
            }
        }
    }

// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 