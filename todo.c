/*          todo - Light TODO list
          Copyright (C)  2012  nCdy

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

#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include <stdlib.h>

void help(char* argv) {
    printf("usage:\n  %s <command>\n  - initdb - init empty database structure\n  - read or r - to read all\n", argv);
    printf("  - write or w <msg> - add task\n  - rm <number> - delete task\n  - clean - clean all tasks\n\r");
    }
int main(int argc, char* argv[]) {
    if (argc < 2) {
        help(argv[0]);
        return 0;
        }
    else {
        if (strcmp(argv[1], "--version") == 0) {
            printf("v0.01\n\r");
            return 0;
            }
        if (strcmp(argv[1], "--help") == 0) {
            help(argv[0]);
            return 0;
            }
        if (strcmp(argv[1], "initdb") == 0
                || strcmp(argv[1], "read") == 0
                || strcmp(argv[1], "r") == 0
                || strcmp(argv[1], "clean") == 0
                || strcmp(argv[1], "rm") == 0
                || strcmp(argv[1], "write") == 0
                || strcmp(argv[1], "w") == 0) {
            int retval, x;
            int q_cnt = 10, q_size = 255, ind = 0;
            char** queries = (char**)malloc(sizeof(char*) * q_cnt);
            for (x = 0; x < q_cnt; x++) {
                queries[x] = (char*)malloc(sizeof(char) * q_size);
                }
            sqlite3_stmt* stmt;
            sqlite3* handle;
            retval = sqlite3_open(strcat(getenv("HOME"), "/.todo.db3"), &handle);
            if (retval) {
                printf("Database connection failed\n\r");
                return -1;
                }
            if (strcmp(argv[1], "initdb") == 0) {
                char create_table[100] = "CREATE TABLE IF NOT EXISTS TODO (id INTEGER PRIMARY KEY,text TEXT NOT NULL)";
                retval = sqlite3_exec(handle, create_table, 0, 0, 0);
                }
            else if ((strcmp(argv[1], "write") == 0) || (strcmp(argv[1], "w") == 0)) {
                if (argc < 3) printf("write what?\n\r");
                else {
                    queries[ind++] = "SELECT MAX(id) FROM TODO";
                    retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
                    if (retval) {
                        printf("Inserting data to DB Failed, run initdb first\n\r");
                        return -1;
                        }
                    int last = 0;
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        last = atoi(sqlite3_column_text(stmt, 0));
                        }
                    last++;
                    int argi;
                    char text[200]; //(char*)calloc(200, sizeof(char));
                    for (argi = 2; argi < argc; argi++) {
                        strcat(text, argv[argi]);
                        strcat(text, " ");
                        }
                    sprintf(queries[ind++], "INSERT INTO TODO VALUES(%d,'%s')", last, text);
                    retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                    if (retval) {
                        printf("Task were not added! (shit happens)\n\r");
                        }
                    }
                }
            else if (strcmp(argv[1], "clean") == 0) {
                queries[ind++] = "DELETE FROM TODO";
                retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                }
            else if (strcmp(argv[1], "rm") == 0) {
                if (argc < 3) printf("rm what?\n\r");
                else {
                    sprintf(queries[ind++], "DELETE FROM TODO WHERE id = %s", argv[2]);
                    retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                    }
                }
            else if ((strcmp(argv[1], "read") == 0) || (strcmp(argv[1], "r") == 0)) {
                int maxl = 0;
                if (argc > 2) sprintf(queries[ind++], "SELECT MAX(LENGTH(text)) FROM TODO WHERE id = %s", argv[2]);
                else queries[ind++] = "SELECT MAX(LENGTH(text)) from TODO";
                retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
                if (retval) {
                    printf("Reading data from DB Failed, run initdb first\n\r");
                    return -1;
                    }
                int last = 0;
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    maxl = atoi(sqlite3_column_text(stmt, 0));
                    }
                if (argc > 2) sprintf(queries[ind++], "SELECT id, text FROM TODO WHERE id = %s", argv[2]);
                else queries[ind++] = "SELECT id, text from TODO";
                retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
                if (retval) {
                    printf("Selecting data from DB Failed, run initdb first\n\r");
                    return -1;
                    }
                char lineborder[255];
                int i;
                for (i = 0; i < (maxl + 7); i++) {
                    strcat(lineborder, "-");
                    }
                printf("+%s+\n\r", lineborder);
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    printf("| %s >> %s\n"
                           , sqlite3_column_text(stmt, 0)
                           , sqlite3_column_text(stmt, 1));
                    }
                printf("+%s+\n\r", lineborder);
                }
            sqlite3_close(handle);
            }
	else {
	    printf("What?\n\n");
	    help(argv[0]);
	    }
        }
    }
