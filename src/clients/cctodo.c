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
//________________________________________________________________________________
#ifdef WIN32
#include <Windows.h>
#endif
//________________________________________________________________________________
#include "ctodo.h"
#include <stdio.h>
#include <string.h>
//________________________________________________________________________________
char* dest;
char* cctodo_version() {
    return "  CCTODO Client v1.0.1\n";
    }
char* cctodo_help() {
    dest = (char*)calloc(4000, sizeof(char));
#ifdef WIN32
    strcpy_s(dest, 4000, cctodo_version());
#else
    strcpy(dest,cctodo_version());
#endif
#ifdef WIN32
    strcat_s(dest, 4000, 
#else
    strcat(dest,
#endif
"  * usage:\n\
    todo <command> <arguments>\n\
  - initdb - init empty database structure\n\
  - set default database options without data lose, useful if you or some update broke it)\n\
  - <msg> - just write todo <msg> to add new node to your todo list\n\
      --first to put task on top priority\n\
      --motivate - end todo note with additional word (see ending option)\n\
  - read or r - to read all\n\
  - edit or e <number> <msg> - edit task\n\
  - mv <number1> <number2> - move task\n\
  - rm <number> - delete task\n\
  - clean - clean all tasks\n\
  - swap or s <number1> <number2> - swap elements\n\
  - sync - text synchronization to avoid binaries in vcs\n\
  - set <option> <value> - todo options, available options:\n\
      - syncdir - directory for vcs synchronization\n\
      - syncfile - file for text serialization for synchronization (default 'readme.md')\n\
        - git - execute git synchronization 1/0 for enable/disable (default 1)\n\
        - hg - execute mercurial synchronization 1/0 for enable/disable (default 0)\n\
        - svn - execute subversion synchronization 1/0 for enable/disable (default 0)\n\
        - vv - execute veracity synchronization 1/0 for enable/disable (default 0)\n\
      - end - always end todo notes with additional word (default 0)\n\
      - ending - word, using for end feature (default 'be a man')\n");
#ifndef WIN32
    strcat(dest,"\
      - home - file for home path (request for synchronization)\n\
      - color - ctodo color scheme for posix (default 'red')\n\
        - schemas: red, blink, green, pink, black\n");
#endif
    return &dest[0];
    }
int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("%s", todo_version());
        printf("%s", cctodo_help());
        return 0;
        }
    else {
        if (strcmp(argv[1], "--version") == 0) {
            printf("%s", cctodo_version());
            printf("%s", todo_version());
            return 0;
            }
        else if (strcmp(argv[1], "--help") == 0) {
            printf("%s", cctodo_help());
            printf("%s", todo_help());
            return 0;
            }
        else if (strcmp(argv[1], "initdb") == 0) {
            todo_initdb();
            return 0;
            }
        else if (strcmp(argv[1], "set") == 0) {
            if (argc < 4) {
                printf("set what?\n\r");
                return 0;
                }
            else {
                return todo_set(argv, argc);
                }
            }
        else if (strcmp(argv[1], "sync") == 0) {
            return todo_sync(argv);
            }
        else if ((strcmp(argv[1], "edit") == 0) || (strcmp(argv[1], "e") == 0)) {
            if (argc < 3) printf("edit what?\n\r");
            else {
                todo_edit(argv, argc);
                }
            return 0;
            }
        else if ((strcmp(argv[1], "swap") == 0) || (strcmp(argv[1], "s") == 0)) {
            if (argc < 3) printf("swap what?\n\r");
            else {
                todo_swap(argv);
                }
            return 0;
            }
        else if (strcmp(argv[1], "mv") == 0) {
            if (argc < 4) printf("move what? you need to provide two indexes\n\r");
            else {
                todo_mv(argv);
                }
            return 0;
            }
        else if (strcmp(argv[1], "clean") == 0) {
            todo_clean();
            return 0;
            }
        else if (strcmp(argv[1], "rm") == 0) {
            if (argc < 3) printf("remove what?\n\r");
            else {
                todo_rm(argv);
                }
            return 0;
            }
        else if ((strcmp(argv[1], "read") == 0) || (strcmp(argv[1], "r") == 0)) {
            char** out;
            int x, maxl;
            out = todo_read(0, 0);
#ifdef WIN32
            memcpy(&maxl, out[1], sizeof(int));
#else
            memcpy(&maxl, out[3], sizeof(int));
#endif
            if (out != NULL) {
                
#ifdef WIN32
                printf("+%s+\n\r", out[0]);
                
#else
                printf(" %s", out[2]);
                printf("╔%s╗", out[0]);
                printf("%c[%dm\n\r", 0x1B, 0);
#endif
#ifdef WIN32
                for(x = 2; x < maxl; x++) {
                    printf("%s", out[x]);
#else
                for(x = 4; x < maxl; x+=2) {
                    printf(" %s║", out[2]);
                    printf("%c[%dm", 0x1B, 0);
                    printf("%s", out[x]);
                    printf("%s│", out[2]);
                    printf("%c[%dm", 0x1B, 0);
                    printf("%s", out[x+1]);
                    printf("%s║", out[2]);
                    printf("%c[%dm\n", 0x1B, 0);
#endif
                }
#ifdef WIN32
                printf("+%s+\n\r", out[0]);
#else
                printf(" %s", out[2]);
                printf("╚%s╝", out[1]);
                printf("%c[%dm\n\r", 0x1B, 0);
#endif
                return 0;
                }
            else {
                return -1;
                }
            }
        else {
            return todo_write(argv, argc);
            }
        }
        todo_close();
    }
//________________________________________________________________________________
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
