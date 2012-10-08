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
#include <time.h>
#include <string.h>
#include <sqlite3.h>
#include <stdlib.h>

int retval, x, q_cnt = 10, q_size = 255, ind = 0;
char** queries;
sqlite3* handle;
void help(char* argv) {
    printf("usage:\n  %s <command>\n  - initdb - init empty database structure\n  - read or r - to read all\n", argv);
    printf("  - write or w <msg> - add task\n  - edit or e <n> <msg> - edit task\n");
    printf("  - rm <number> - delete task\n  - clean - clean all tasks\n\r");
    printf("  - sync - text synchronization to avoid binaries in vcs\n\r");
    }
void timeUpdate(time_t t) {
    sprintf(queries[ind++], "INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (1,'%d')", (int)t);
    retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
    if (retval) {
        printf("Failed to update db time\n\r");
        return;
        }
    }
char* rtrim(char* str) {
    char* ptr;
    int len;
    len = strlen(str);
    for (ptr = str + len - 1; ptr >= str && isspace((int)*ptr); --ptr);
    ptr[1] = '\0';
    return str;
    }
int main(int argc, char* argv[]) {
    if (argc < 2) {
        help(argv[0]);
        return 0;
        }
    else {
        if (strcmp(argv[1], "--version") == 0) {
            printf("v1.00\n\r");
            return 0;
            }
        if (strcmp(argv[1], "--help") == 0) {
            help(argv[0]);
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
            queries = (char**)malloc(sizeof(char*) * q_cnt);
            for (x = 0; x < q_cnt; x++) {
                queries[x] = (char*)malloc(sizeof(char) * q_size);
                }
            sqlite3_stmt* stmt;
            retval = sqlite3_open(strcat(getenv("HOME"), "/.todo.db3"), &handle);
            if (retval) {
                printf("Database connection failed\n\r");
                return -1;
                }
            if (strcmp(argv[1], "initdb") == 0) {
                sprintf(queries[ind++], "CREATE TABLE IF NOT EXISTS TODO (id INTEGER PRIMARY KEY,text TEXT NOT NULL)");
                retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                if (retval) {
                    printf("Init DB Failed, Shit happens?\n\r");
                    return -1;
                    }
                sprintf(queries[ind++], "CREATE TABLE IF NOT EXISTS OPTIONS (option INTEGER PRIMARY KEY,text TEXT NOT NULL)");
                retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                sprintf(queries[ind++], "INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (0,'.todo.sync')");
                retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                sprintf(queries[ind++], "INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (1,'%d')", (int)(time(0)));
                retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                if (retval) {
                    printf("Instert deafaults options Failed, Shit happens?\n\r");
                    return -1;
                    }
                }
            if (strcmp(argv[1], "sync") == 0) {
                char* filename;
                int timeDB;
                filename = (char*)calloc(200, sizeof(char));
                sprintf(queries[ind++], "SELECT option, text FROM OPTIONS WHERE option = 0 OR option = 1");
                retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
                if (retval) {
                    printf("Sync data Failed, run initdb first\n\r");
                    return -1;
                    }
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    if (strcmp(sqlite3_column_text(stmt, 0), "0") == 0) {
                        sprintf(filename, "%s", sqlite3_column_text(stmt, 1));
                        }
                    else {
                        timeDB = atoi(sqlite3_column_text(stmt, 1));
                        }
                    }
                printf("Sync file: %s\n\r", filename);
                FILE* f = fopen(filename, "a+");
                if (f == NULL) {
                    printf("There is no such file and it's failed to create it\n\r");
                    return -1;
                    }
                int i = 0, timefile = 0;
                char line[150];
                char write = 1;
                char* token1;
                char* token2;
                char* search = "|";
                while (fgets(line, 150, f)) {
                    if (i == 0) {
                        timefile = atoi(line);
                        printf("Timefile: %d\n\r", timefile);
                        if (timeDB > timefile) {
                            break;
                            }
                        else write = 0;
                        }
                    else {
                        if (i == 1) {
                            queries[ind++] = "DELETE FROM TODO";
                            retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                            if (retval) {
                                printf("failed to clean db, run initdb first\n\r");
                                }
                            }
                        token1 = strtok(line, search);
                        token2 = strtok(NULL, search);
                        rtrim(token2);
                        sprintf(queries[ind++], "INSERT INTO TODO VALUES(%s,'%s')", token1, token2);
                        retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                        if (retval) {
                            printf("Task were not added! (shit happens)\n\r");
                            }
                        }
                    i++;
                    }
                time_t now = time(0);
                timeUpdate(now);
                printf("\n");
                fclose(f);
                if (write) {
                    f = fopen(filename, "w+");
                    rewind(f);
                    queries[ind++] = "SELECT id, text from TODO";
                    retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
                    fprintf(f, "%d\n", (int)now);
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        fprintf(f, "%s|%s\n"
                                , sqlite3_column_text(stmt, 0)
                                , sqlite3_column_text(stmt, 1));
                        }
                    fclose(f);
                    }
                free(filename);
                }
            else if ((strcmp(argv[1], "write") == 0) || (strcmp(argv[1], "w") == 0)) {
                if (argc < 3) printf("write what?\n\r");
                else {
                    sprintf(queries[ind++], "SELECT COALESCE(MAX(id),0) FROM TODO");
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
                    char* text;
                    text = (char*)calloc(200, sizeof(char));
                    for (argi = 2; argi < argc; argi++) {
                        strcat(text, argv[argi]);
                        strcat(text, " ");
                        }
                    sprintf(queries[ind++], "INSERT INTO TODO VALUES(%d,'%s')", last, text);
                    retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                    if (retval) {
                        printf("Task were not added! (shit happens)\n\r");
                        return -1;
                        }
                    free(text);
                    timeUpdate(time(0));
                    }
                }
            else if ((strcmp(argv[1], "edit") == 0) || (strcmp(argv[1], "e") == 0)) {
                if (argc < 3) printf("edit what?\n\r");
                else {
                    int argi;
                    char* text;
                    text = (char*)calloc(200, sizeof(char));
                    for (argi = 2; argi < argc; argi++) {
                        strcat(text, argv[argi]);
                        strcat(text, " ");
                        }
                    sprintf(queries[ind++], "UPDATE TODO SET text='%s' WHERE id = %s", text, argv[2]);
                    retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                    if (retval) {
                        printf("Task were not edited! (shit happens)\n\r");
                        }
                    free(text);
                    timeUpdate(time(0));
                    }
                }
            else if (strcmp(argv[1], "clean") == 0) {
                queries[ind++] = "DELETE FROM TODO";
                retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                timeUpdate(time(0));
                }
            else if (strcmp(argv[1], "rm") == 0) {
                if (argc < 3) printf("rm what?\n\r");
                else {
                    sprintf(queries[ind++], "DELETE FROM TODO WHERE id = %s", argv[2]);
                    retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                    timeUpdate(time(0));
                    }
                }
            else if ((strcmp(argv[1], "read") == 0) || (strcmp(argv[1], "r") == 0)) {
                int maxl2 = 0, maxl1 = 0;
                if (argc > 2) sprintf(queries[ind++], "SELECT COALESCE(MAX(id),0) FROM TODO WHERE id = %s", argv[2]);
                else queries[ind++] = "SELECT COALESCE(MAX(id),0) from TODO";
                retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
                if (retval) {
                    printf("Reading data from DB Failed, run initdb first\n\r");
                    return -1;
                    }
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    maxl1 = strlen(sqlite3_column_text(stmt, 0));
                    }
                if (argc > 2) sprintf(queries[ind++], "SELECT COALESCE(MAX(LENGTH(text)),0) FROM TODO WHERE id = %s", argv[2]);
                else queries[ind++] = "SELECT COALESCE(MAX(LENGTH(text)),0) from TODO";
                retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
                if (retval) {
                    printf("Reading data from DB Failed, run initdb first\n\r");
                    return -1;
                    }
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    maxl2 = atoi(sqlite3_column_text(stmt, 0));
                    }
                if (argc > 2) sprintf(queries[ind++], "SELECT id, text, LENGTH(text) FROM TODO WHERE id = %s", argv[2]);
                else queries[ind++] = "SELECT id, text, LENGTH(text) from TODO";
                retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
                if (retval) {
                    printf("Selecting data from DB Failed, run initdb first\n\r");
                    return -1;
                    }
                char* lineborder;
                char* spaces1;
                char* spaces2;
                lineborder = (char*)calloc(255, sizeof(char));
                spaces1    = (char*)calloc(200, sizeof(char));
                spaces2    = (char*)calloc(200, sizeof(char));
                int i, maxi1, maxi2;
                for (i = 0; i < ((maxl2 + maxl1) + 6); i++) {
                    strcat(lineborder, "-");
                    }
                printf("+%s+\n\r", lineborder);
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    maxi1 = maxl1 - strlen(sqlite3_column_text(stmt, 0));
                    maxi2 = maxl2 - atoi(sqlite3_column_text(stmt, 2));
                    strcpy(spaces1, "");
                    strcpy(spaces2, "");
                    for (i = 0; i < maxi1; i++) {
                        strcat(spaces1, " ");
                        }
                    for (i = 0; i < maxi2; i++) {
                        strcat(spaces2, " ");
                        }
                    printf("| %s %s>> %s %s|\n"
                           , sqlite3_column_text(stmt, 0)
                           , spaces1
                           , sqlite3_column_text(stmt, 1)
                           , spaces2);
                    }
                printf("+%s+\n\r", lineborder);
                free(lineborder);
                free(spaces1);
                free(spaces2);
                }
            sqlite3_close(handle);
            }
        else {
            printf("What?\n\n");
            help(argv[0]);
            }
        }
    }
