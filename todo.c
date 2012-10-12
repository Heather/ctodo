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

#ifdef WIN32
#include "sqlite3.h"
#else
#include <sqlite3.h>
#endif
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int retval, x, q_cnt = 10, q_size = 255, ind = 0;
char** queries;
sqlite3* handle;
void help(char* argv) {
    printf("CToDo: CLI TODO List Management Util\n\r");
    printf(" * usage:\n\r");
    printf("  %s <command> <arguments>\n\r", argv);
    printf("  - initdb - init empty database structure\n\r");
    printf("  - read or r - to read all\n\r");
    printf("  - write or w <msg> - add task\n\r");
    printf("  - edit or e <n> <msg> - edit task\n\r");
    printf("  - rm <number> - delete task\n\r");
    printf("  - clean - clean all tasks\n\r");
    printf("  - swap <number1> <number2> - swap elements\n\r");
    printf("  - set <option> <value> - todo options, available options:\n\r");
    printf("      - syncfile - file for text serialization for synchronization (default '.todo.sync')\n\r");
#ifndef WIN32
    printf("      - home - file for home path (request for synchronization)\n\r");
#endif
    printf("      - git - execute git synchronization 1/0 for enable/disable (default 1)\n\r");
    printf("      - hg - execute mercurial synchronization 1/0 for enable/disable (default 1)\n\r");
    printf("      - svn - execute subversion synchronization 1/0 for enable/disable (default 1)\n\r");
    printf("  - sync - text synchronization to avoid binaries in vcs\n\r");
    }
void timeUpdate(time_t t) {
#ifdef WIN32
    sprintf_s(queries[ind++], 255, "INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (1,'%d')", (int)t);
#else
    sprintf(queries[ind++], "INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (1,'%d')", (int)t);
#endif
    retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
    if (retval) {
        printf("Failed to update db time\n\r");
        return;
        }
    }
void sql(char* command) {
#ifdef WIN32
    sprintf_s(queries[ind++], 255, "%s", command);
#else
    sprintf(queries[ind++], "%s", command);
#endif
    retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
    }
char* rtrim(char* str) {
    char* ptr;
    int len;
    len = strlen(str);
    for (ptr = str + len - 1; ptr >= str && isspace((int)*ptr); --ptr);
    ptr[1] = '\0';
    return str;
    }
void close() {
    sqlite3_close(handle);
    }
int main(int argc, char* argv[]) {
    sqlite3_stmt* stmt;
    time_t timefile = 0;
    FILE* f = NULL;
    if (argc < 2) {
        help(argv[0]);
        return 0;
        }
    else {
        if (strcmp(argv[1], "--version") == 0) {
            printf("v1.01\n\r");
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
                || strcmp(argv[1], "set") == 0
                || strcmp(argv[1], "swap") == 0
                || strcmp(argv[1], "sync") == 0) {
            queries = (char**)malloc(sizeof(char*) * q_cnt);
            for (x = 0; x < q_cnt; x++) {
                queries[x] = (char*)malloc(sizeof(char) * q_size);
                }
#ifdef WIN32
            retval = sqlite3_open("todo.db3", &handle);
#else
            retval = sqlite3_open(strcat(getenv("HOME"), "/.todo.db3"), &handle);
#endif
            if (retval) {
                printf("Database connection failed\n\r");
                return -1;
                }
            if (strcmp(argv[1], "initdb") == 0) {
#ifndef WIN32
                char* home = (char*)getenv("HOME");
#endif
                sql("CREATE TABLE IF NOT EXISTS TODO (id INTEGER PRIMARY KEY,text TEXT NOT NULL)");
                if (retval) {
                    printf("Init DB Failed, Shit happens?\n\r");
                    close();
                    return -1;
                    }
                sql("CREATE TABLE IF NOT EXISTS OPTIONS (option INTEGER PRIMARY KEY,text TEXT NOT NULL)");

                ///<Option>
                ///Sync file for tex serialization
                ///</Option>
                sql("INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (0,'readme.md')");
                ///<Option>
                ///Time of last synchronization
                ///</Option>
#ifdef WIN32
                sprintf_s(queries[ind++], 255, "INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (1,'%d')", (int)(time(0)));
#else
                sprintf(queries[ind++], "INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (1,'%d')", (int)(time(0)));
#endif
                retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                ///<Option>
                ///Use git for synchronization
                ///</Option>
                sql("INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (2,'1')");
                ///<Option>
                ///Use mercurial (hg) for synchronization
                ///</Option>
                sql("INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (3,'0')");
                ///<Option>
                ///Use subversion (svn) for synchronization
                ///</Option>
                sql("INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (4,'0')");
                ///<Option>
                ///Path for HOME (only for linux)
                ///</Option>
#ifndef WIN32
                sprintf(queries[ind++], "INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (20,'%s')", home);
                retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
#endif
                if (retval) {
                    printf("Instert deafaults options Failed, Shit happens?\n\r");
                    close();
                    return -1;
                    }
                }
            if (strcmp(argv[1], "set") == 0) {
                if (argc < 4) printf("set what?\n\r");
                else {
                    if (strcmp(argv[2], "syncfile") == 0) {
#ifdef WIN32
                        sprintf_s(queries[ind++], 255, "UPDATE OPTIONS SET text='%s' WHERE option = 0", argv[3]);
#else
                        sprintf(queries[ind++], "UPDATE OPTIONS SET text='%s' WHERE option = 0", argv[3]);
#endif
                        retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                        if (retval) {
                            printf("Option syncfile is not changed! (shit happens)\n\r");
                            close();
                            return -1;
                            }
                        }
                    if ((strcmp(argv[2], "git") == 0)
                            || (strcmp(argv[2], "hg") == 0)
                            || (strcmp(argv[2], "svn") == 0)) {
                        if ((strcmp(argv[3], "1") == 0) || (strcmp(argv[3], "0") == 0)) {
                            if (strcmp(argv[2], "git") == 0) {
#ifdef WIN32
                                sprintf_s(queries[ind++], 255, "UPDATE OPTIONS SET text='%s' WHERE option = 2", argv[3]);
#else
                                sprintf(queries[ind++], "UPDATE OPTIONS SET text='%s' WHERE option = 2", argv[3]);
#endif
                                }
                            else if (strcmp(argv[2], "hg") == 0) {
#ifdef WIN32
                                sprintf_s(queries[ind++], 255, "UPDATE OPTIONS SET text='%s' WHERE option = 3", argv[3]);
#else
                                sprintf(queries[ind++], "UPDATE OPTIONS SET text='%s' WHERE option = 3", argv[3]);
#endif
                                }
                            else if (strcmp(argv[2], "svn") == 0) {
#ifdef WIN32
                                sprintf_s(queries[ind++], 255, "UPDATE OPTIONS SET text='%s' WHERE option = 4", argv[3]);
#else
                                sprintf(queries[ind++], "UPDATE OPTIONS SET text='%s' WHERE option = 4", argv[3]);
#endif
                                }
                            retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                            if (retval) {
                                printf("Option is not changed! (shit happens)\n\r");
                                close();
                                return -1;
                                }
                            }
                        else {
                            printf("Use 1 or 0 for this option\n\r");
                            }
                        }
#ifndef WIN32
                    if (strcmp(argv[2], "home") == 0) {
                        sprintf(queries[ind++], "UPDATE OPTIONS SET text='%s' WHERE option = 20", argv[3]);
                        retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                        if (retval) {
                            printf("Option home is not changed! (shit happens)\n\r");
                            close();
                            return -1;
                            }
                        }
#endif
                    }
                }
            if (strcmp(argv[1], "sync") == 0) {
                char* filename;
#ifndef WIN32
                char* home = "HOME=";
#endif
                int timeDB;
                int git, hg, svn;
                int i = 0;
                char line[150];
                char write = 1;
                char* token1;
                char* token2;
                char* search = "|";
#ifdef WIN32
                char* context = NULL;
#endif
                filename = (char*)calloc(200, sizeof(char));
#ifdef WIN32
                sprintf_s(queries[ind++], 255, "SELECT option, text FROM OPTIONS");
#else
                sprintf(queries[ind++], "SELECT option, text FROM OPTIONS");
#endif
                retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
                if (retval) {
                    printf("Sync data Failed, run initdb first\n\r");
                    return -1;
                    }
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    if (strcmp((const char*)sqlite3_column_text(stmt, 0), "0") == 0) {
#ifdef WIN32
                        sprintf_s(filename, 200, "%s", sqlite3_column_text(stmt, 1));
#else
                        sprintf(filename, "%s", sqlite3_column_text(stmt, 1));
#endif
                        }
                    else if (strcmp((const char*)sqlite3_column_text(stmt, 0), "1") == 0) {
                        timeDB = atoi((const char*)sqlite3_column_text(stmt, 1));
                        }
                    else if (strcmp((const char*)sqlite3_column_text(stmt, 0), "2") == 0) {
                        git = atoi((const char*)sqlite3_column_text(stmt, 1));
                        }
                    else if (strcmp((const char*)sqlite3_column_text(stmt, 0), "3") == 0) {
                        hg = atoi((const char*)sqlite3_column_text(stmt, 1));
                        }
                    else if (strcmp((const char*)sqlite3_column_text(stmt, 0), "4") == 0) {
                        svn = atoi((const char*)sqlite3_column_text(stmt, 1));
                        }
                    else if (strcmp((const char*)sqlite3_column_text(stmt, 0), "20") == 0) {
#ifndef WIN32
                        strcat(home, sqlite3_column_text(stmt, 1));
#endif
                        }
                    }
                if (git == 1 || hg == 1 || svn == 1) {
#ifndef WIN32
                    putenv(home);
#endif
                    if (git == 1) {
                        if (system("git pull") == -1) return -1;
                        }
                    else if (hg == 1) {
                        if (system("hg pull --update") == -1) return -1;
                        }
                    else if (svn == 1) {
                        if (system("svn update") == -1) return -1;
                        }
                    }
                printf("Sync file: %s\n\r", filename);
#ifdef WIN32
                fopen_s(&f, filename, "a+");
#else
                f = fopen(filename, "a+");
#endif
                if (f == NULL) {
                    printf("There is no such file and it's failed to create it\n\r");
                    close();
                    return -1;
                    }
                while (fgets(line, 150, f)) {
                    if (i == 0) {
                        timefile = atoi(line);
#ifndef WIN32
                        printf("Timefile: %s\n\r", ctime(&timefile));
#endif
                        if (timeDB > (int)timefile) {
                            break;
                            }
                        else if (timeDB == (int)timefile) {
                            printf("Everything is up to date\n\r");
                            close();
                            return 0;
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
                        else {
#ifdef WIN32
                            token1 = strtok_s(line, search, &context);
                            token2 = strtok_s(NULL, search, &context);
#else
                            token1 = strtok(line, search);
                            token2 = strtok(NULL, search);
#endif
                            if (token1[1] == '-') token1 += 3;
                            rtrim(token2);
#ifdef WIN32
                            sprintf_s(queries[ind++], 255, "INSERT INTO TODO VALUES(%s,'%s')", token1, token2);
#else
                            sprintf(queries[ind++], "INSERT INTO TODO VALUES(%s,'%s')", token1, token2);
#endif
                            retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                            if (retval) {
                                printf("Task were not added! (shit happens)\n\r");
                                }
                            }
                        }
                    i++;
                    }
                fclose(f);
                if (write) {
                    time_t now = time(0);
#ifdef WIN32
                    fopen_s(&f, filename, "w+");
#else
                    f = fopen(filename, "w+");
#endif
                    rewind(f);
                    queries[ind++] = "SELECT id, text from TODO";
                    retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
                    fprintf(f, "%d\n", (int)now);
                    fprintf(f, "\n");
                    timeUpdate(now);
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        fprintf(f, " - %s|%s\n"
                                , sqlite3_column_text(stmt, 0)
                                , sqlite3_column_text(stmt, 1));
                        }
                    fclose(f);
                    if (git == 1 || hg == 1 || svn == 1) {
#ifndef WIN32
                        putenv(home);
#endif
                        if (git == 1) {
                            if (system("git commit -am \"TODO LIST UPDATE\"") == -1) return -1;
                            if (system("git push") == -1) return -1;
                            }
                        else if (hg == 1) {
                            if (system("hg commit -m \"TODO LIST UPDATE\"") == -1) return -1;
                            if (system("hg push") == -1) return -1;
                            }
                        else if (svn == 1) {
                            if (system("svn commit  -m \"TODO LIST UPDATE\"") == -1) return -1;
                            }
                        }
                    printf("synchronization complete, syncfile updated\n\r");
                    }
                else {
                    timeUpdate(timefile);
                    printf("synchronization complete, local database updated\n\r");
                    }
                free(filename);
                }
            else if ((strcmp(argv[1], "write") == 0) || (strcmp(argv[1], "w") == 0)) {
                if (argc < 3) printf("write what?\n\r");
                else {
                    int last = 0;
                    int argi;
                    char* text;
#ifdef WIN32
                    sprintf_s(queries[ind++], 255, "SELECT COALESCE(MAX(id),0) FROM TODO");
#else
                    sprintf(queries[ind++], "SELECT COALESCE(MAX(id),0) FROM TODO");
#endif
                    retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
                    if (retval) {
                        printf("Inserting data to DB Failed, run initdb first\n\r");
                        return -1;
                        }
                    while (sqlite3_step(stmt) == SQLITE_ROW) {
                        last = atoi((const char*)sqlite3_column_text(stmt, 0));
                        }
                    last++;
                    text = (char*)calloc(200, sizeof(char));
                    for (argi = 2; argi < argc; argi++) {
#ifdef WIN32
                        strcat_s(text, 200, argv[argi]);
                        strcat_s(text, 200, " ");
#else
                        strcat(text, argv[argi]);
                        strcat(text, " ");
#endif
                        }
#ifdef WIN32
                    sprintf_s(queries[ind++], 255, "INSERT INTO TODO VALUES(%d,'%s')", last, text);
#else
                    sprintf(queries[ind++], "INSERT INTO TODO VALUES(%d,'%s')", last, text);
#endif
                    retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                    if (retval) {
                        printf("Task were not added! (shit happens)\n\r");
                        close();
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
                    for (argi = 3; argi < argc; argi++) {
#ifdef WIN32
                        strcat_s(text, 200, argv[argi]);
                        strcat_s(text, 200, " ");
#else
                        strcat(text, argv[argi]);
                        strcat(text, " ");
#endif
                        }
#ifdef WIN32
                    sprintf_s(queries[ind++], 255, "UPDATE TODO SET text='%s' WHERE id = %s", text, argv[2]);
#else
                    sprintf(queries[ind++], "UPDATE TODO SET text='%s' WHERE id = %s", text, argv[2]);
#endif
                    retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                    if (retval) {
                        printf("Task were not edited! (shit happens)\n\r");
                        }
                    free(text);
                    timeUpdate(time(0));
                    }
                }
            else if (strcmp(argv[1], "swap") == 0) {
                if (argc < 3) printf("swap what?\n\r");
                else {
                    int val1 = atoi(argv[2]);
                    int val2 = atoi(argv[3]);
#ifdef WIN32
                    sprintf_s(queries[ind++], 255, "UPDATE TODO SET id=%d WHERE id = %d", 9999, val1);
#else
                    sprintf(queries[ind++], "UPDATE TODO SET id=%d WHERE id = %d", 9999, val1);
#endif
                    retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                    if (retval) {
                        printf("Swap failed! (shit happens)\n\r");
                        }
#ifdef WIN32
                    sprintf_s(queries[ind++], 255, "UPDATE TODO SET id=%d WHERE id = %d", val1, val2);
#else
                    sprintf(queries[ind++], "UPDATE TODO SET id=%d WHERE id = %d", val1, val2);
#endif
                    retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                    if (retval) {
                        printf("Swap failed! (shit happens)\n\r");
                        }
#ifdef WIN32
                    sprintf_s(queries[ind++], 255, "UPDATE TODO SET id=%d WHERE id = %d", val2, 9999);
#else
                    sprintf(queries[ind++], "UPDATE TODO SET id=%d WHERE id = %d", val2, 9999);
#endif
                    retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                    if (retval) {
                        printf("Swap failed! (shit happens)\n\r");
                        }
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
#ifdef WIN32
                    sprintf_s(queries[ind++], 255, "DELETE FROM TODO WHERE id = %s", argv[2]);
#else
                    sprintf(queries[ind++], "DELETE FROM TODO WHERE id = %s", argv[2]);
#endif
                    retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                    timeUpdate(time(0));
                    }
                }
            else if ((strcmp(argv[1], "read") == 0) || (strcmp(argv[1], "r") == 0)) {
                char* lineborder1;
                char* lineborder2;
                char* spaces1;
                char* spaces2;
                int maxl2 = 0, maxl1 = 0;
                int i, maxi1, maxi2;
                if (argc > 2) {
#ifdef WIN32
                    sprintf_s(queries[ind++], 255, "SELECT COALESCE(MAX(id),0) FROM TODO WHERE id = %s", argv[2]);
#else
                    sprintf(queries[ind++], "SELECT COALESCE(MAX(id),0) FROM TODO WHERE id = %s", argv[2]);
#endif
                    }
                else queries[ind++] = "SELECT COALESCE(MAX(id),0) from TODO";
                retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
                if (retval) {
                    printf("Reading data from DB Failed, run initdb first\n\r");
                    return -1;
                    }
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    maxl1 = strlen((const char*)sqlite3_column_text(stmt, 0));
                    }
                if (argc > 2) {
#ifdef WIN32
                    sprintf_s(queries[ind++], 255, "SELECT COALESCE(MAX(LENGTH(text)),0) FROM TODO WHERE id = %s", argv[2]);
#else
                    sprintf(queries[ind++], "SELECT COALESCE(MAX(LENGTH(text)),0) FROM TODO WHERE id = %s", argv[2]);
#endif
                    }
                else queries[ind++] = "SELECT COALESCE(MAX(LENGTH(text)),0) from TODO";
                retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
                if (retval) {
                    printf("Reading data from DB Failed, run initdb first\n\r");
                    return -1;
                    }
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    maxl2 = atoi((const char*)sqlite3_column_text(stmt, 0));
                    }
                if (argc > 2) {
#ifdef WIN32
                    sprintf_s(queries[ind++], 255, "SELECT id, text, LENGTH(text) FROM TODO WHERE id = %s", argv[2]);
#else
                    sprintf(queries[ind++], "SELECT id, text, LENGTH(text) FROM TODO WHERE id = %s", argv[2]);
#endif
                    }
                else queries[ind++] = "SELECT id, text, LENGTH(text) from TODO";
                retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
                if (retval) {
                    printf("Selecting data from DB Failed, run initdb first\n\r");
                    return -1;
                    }
                lineborder1 = (char*)calloc(255, sizeof(char));
                lineborder2 = (char*)calloc(255, sizeof(char));
                spaces1    = (char*)calloc(200, sizeof(char));
                spaces2    = (char*)calloc(200, sizeof(char));
                for (i = 0; i < ((maxl2 + maxl1) + 5); i++) {
                    if (i == 2 + maxl1) {
#ifdef WIN32
                        strcat_s(lineborder1, 200, "+");
                        strcat_s(lineborder2, 200, "+");
#else
                        strcat(lineborder1, "╤");
                        strcat(lineborder2, "╧");
#endif
                        }
                    else {
#ifdef WIN32
                        strcat_s(lineborder1, 200, "-");
                        strcat_s(lineborder2, 200, "-");
#else
                        strcat(lineborder1, "═");
                        strcat(lineborder2, "═");
#endif
                        }
                    }
#ifdef WIN32
                printf("+%s+\n\r", lineborder1);
#else
                printf(" %c[%d;%d;%dm", 0x1B, 1, 37, 41);
                printf("╔%s╗", lineborder1);
                printf("%c[%dm\n\r", 0x1B, 0); // 27
#endif
                while (sqlite3_step(stmt) == SQLITE_ROW) {
                    maxi1 = maxl1 - strlen((const char*)sqlite3_column_text(stmt, 0));
                    maxi2 = maxl2 - atoi((const char*)sqlite3_column_text(stmt, 2));
#ifdef WIN32
                    strcpy_s(spaces1, 200, "");
                    strcpy_s(spaces2, 200, "");
#else
                    strcpy(spaces1, "");
                    strcpy(spaces2, "");
#endif
                    for (i = 0; i < maxi1; i++) {
#ifdef WIN32
                        strcat_s(spaces1, 200, " ");
#else
                        strcat(spaces1, " ");
#endif
                        }
                    for (i = 0; i < maxi2; i++) {
#ifdef WIN32
                        strcat_s(spaces2, 200, " ");
#else
                        strcat(spaces2, " ");
#endif
                        }
#ifdef WIN32
                    printf("| %s %s| %s %s|\n\r",
                           sqlite3_column_text(stmt, 0)
                           , spaces1
                           , sqlite3_column_text(stmt, 1)
                           , spaces2);
#else
                    printf(" %c[%d;%d;%dm║", 0x1B, 1, 37, 41);
                    printf("%c[%dm", 0x1B, 0);
                    printf(" %s %s",
                           sqlite3_column_text(stmt, 0)
                           , spaces1);
                    printf("%c[%d;%d;%dm│", 0x1B, 1, 37, 41);
                    printf("%c[%dm", 0x1B, 0);
                    printf(" %s %s"
                           , sqlite3_column_text(stmt, 1)
                           , spaces2);
                    printf("%c[%d;%d;%dm║", 0x1B, 1, 37, 41);
                    printf("%c[%dm\n", 0x1B, 0);
#endif
                    }
#ifdef WIN32
                printf("+%s+\n\r", lineborder2);
#else
                printf(" %c[%d;%d;%dm", 0x1B, 1, 37, 41);
                printf("╚%s╝", lineborder2);
                printf("%c[%dm\n\r", 0x1B, 0);
#endif
                free(lineborder1);
                free(lineborder2);
                free(spaces1);
                free(spaces2);
                }
            close();
            }
        else {
            printf("What?\n\n");
            help(argv[0]);
            }
        }
    }
