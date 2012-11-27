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
#include "sqlite3.h"
#else
#include <sqlite3.h>
#endif
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
//________________________________________________________________________________
//General properties _____________________________________________________________
int retval, x, q_size = 255, ind = 0;
char* dest;
char** out;
//Pointers _______________________________________________________________________
FILE* f;
time_t timefile;
sqlite3_stmt* stmt;
//________________________________________________________________________________
//Maximum sqlite query queue _____________________________________________________
#ifdef WIN32
int q_cnt = 11;
#else
char* home;
int q_cnt = 13;
#endif
//________________________________________________________________________________
//Sqlite handlers ________________________________________________________________
char** queries;
sqlite3* handle;
//________________________________________________________________________________
char* todo_version() {
    return "  CTODO List Management Uti v1.1.0\n";
    }
//________________________________________________________________________________
char* todo_help() {
    dest = (char*)calloc(4000, sizeof(char));
#ifdef WIN32
    strcpy_s(dest, 4000, todo_version());
#else
    strcpy(dest,todo_version());
#endif
#ifdef WIN32
    strcat_s(dest, 4000, 
#else
    strcat(dest,
#endif
"  cross-platform ctodo library\n");
    return &dest[0];
    }
//________________________________________________________________________________
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
//________________________________________________________________________________
void sql(char* command) {
#ifdef WIN32
    sprintf_s(queries[ind++], 255, "%s", command);
#else
    sprintf(queries[ind++], "%s", command);
#endif
    retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
    }
//________________________________________________________________________________
char* rtrim(char* str) {
    char* ptr;
    int len;
    len = strlen(str);
    for (ptr = str + len - 1; ptr >= str && isspace((int)*ptr); --ptr);
    ptr[1] = '\0';
    return str;
    }
//________________________________________________________________________________
int prelude() {
    timefile = 0;
    f = NULL;
#ifndef WIN32
    char* temp = (char*)calloc(200, sizeof(char));
#endif
    queries = (char**)malloc(sizeof(char*) * q_cnt);
    for (x = 0; x < q_cnt; x++) {
        queries[x] = (char*)malloc(sizeof(char) * q_size);
        }
#ifdef WIN32
    retval = sqlite3_open("todo.db3", &handle);
#else
    home = (char*)getenv("HOME");
    strcpy(temp, home);
    retval = sqlite3_open(strcat(temp, "/.todo.db3"), &handle);
    free(temp);
#endif
    if (retval) {
        printf("Database connection failed\n\r");
        return -1;
        }
    return 0;
    }
//________________________________________________________________________________
void close() {
    free(dest);
    free(queries);
    sqlite3_close(handle);
    }
void todo_close() {
#ifndef WIN32
    free(home);
#endif
    free(out);
    }
//________________________________________________________________________________
int todo_initdb() {
    if (prelude() == -1) return -1;
    sql("CREATE TABLE IF NOT EXISTS TODO (id INTEGER PRIMARY KEY,text TEXT NOT NULL)");
    if (retval) {
        printf("Init DB Failed, Shit happens?\n\r");
        return -1;
        }
    sql("CREATE TABLE IF NOT EXISTS OPTIONS (option INTEGER PRIMARY KEY,text TEXT NOT NULL)");

    ///<Option>
    ///Sync file for tex serialization
    ///</Option>
    sql("INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (15,'readme.md')");
    ///<Option>
    ///Time of last synchronization
    ///</Option>
#ifdef WIN32
    sprintf_s(queries[ind++], 255, "INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (1,'%d')", 0);
#else
    sprintf(queries[ind++], "INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (1,'%d')", 0);
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
    ///Use veracity (vv) for synchronization
    ///</Option>
    sql("INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (5'0')");
    ///<Option>
    ///Add ending word to each todo row
    ///</Option>
    sql("INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (12,'0')");
    ///<Option>
    ///Ending word
    ///</Option>
    sql("INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (13,'be a man')");
    ///<Option>
    ///Synchronization directory
    ///</Option>
#ifndef WIN32
    sql("INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (0,'/home/nen/todo')");
#else
    sql("INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (0,'.')");
#endif
#ifndef WIN32
    ///<Option>
    ///Path for HOME (only for linux)
    ///</Option>
    sprintf(queries[ind++], "INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (20,'%s')", home);
    retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
    ///<Option>
    ///Color scheme (only for linux)
    ///</Option>
    sql("INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (21,'red')");
    if (retval) {
        printf("Instert deafaults options Failed, Shit happens?\n\r");
        return -1;
        }
#endif
    return 0;
    }
//________________________________________________________________________________
int todo_set(char** argv, int argc) {
    int opt = 0;
    if (prelude() == -1) return -1;
    if (argc < 4) printf("set what?\n\r");
    else {
        if (strcmp(argv[2], "syncfile") == 0) {
            opt = 15;
            }
        else if (strcmp(argv[2], "syncdir") == 0) {
            opt = 0;
            }
        else if (strcmp(argv[2], "ending") == 0) {
            opt = 13;
            }
        else if (strcmp(argv[2], "color") == 0) {
            opt = 21;
            }
        else if (strcmp(argv[2], "end") == 0) {
            if ((strcmp(argv[3], "1") == 0) || (strcmp(argv[3], "0") == 0)) {
                opt = 12;
                }
            else {
                printf("Use 1 or 0 for this option\n\r");
                }
            }
        else if ((strcmp(argv[2], "git") == 0)
                 || (strcmp(argv[2], "hg") == 0)
                 || (strcmp(argv[2], "svn") == 0)
                 || (strcmp(argv[2], "vv") == 0)) {
            if ((strcmp(argv[3], "1") == 0) || (strcmp(argv[3], "0") == 0)) {
                if (strcmp(argv[2], "git") == 0) {
                    opt = 2;
                    }
                else if (strcmp(argv[2], "hg") == 0) {
                    opt = 3;
                    }
                else if (strcmp(argv[2], "svn") == 0) {
                    opt = 4;
                    }
                else if (strcmp(argv[2], "vv") == 0) {
                    opt = 5;
                    }
                }
            else {
                printf("Use 1 or 0 for this option\n\r");
                }
            }
#ifndef WIN32
        else if (strcmp(argv[2], "home") == 0) {
            sprintf(queries[ind++], "UPDATE OPTIONS SET text='%s' WHERE option = 20", argv[3]);
            retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
            if (retval) {
                printf("Option home is not changed! (shit happens)\n\r");
                return -1;
                }
            }
#endif
        else {
            printf("There is no such option\n\r");
            return 0;
            }
#ifdef WIN32
        sprintf_s(queries[ind++], 255, "UPDATE OPTIONS SET text='%s' WHERE option = %d", argv[3], opt);
#else
        sprintf(queries[ind++], "UPDATE OPTIONS SET text='%s' WHERE option = %d", argv[3], opt);
#endif
        retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
        if (retval) {
            printf("Option is not changed! (shit happens)\n\r");
            return -1;
            }
        }
    return 0;
    }
//________________________________________________________________________________
int todo_sync(char** argv) {
    char* filename;
#ifndef WIN32
    char* home;
#endif
    int timeDB;
    int git, hg, svn, vv;
    int i = 0;
    char line[150];
    char write = 1;
    char* token1;
    char* token2;
    char* search = "|";
    char* syncdir;
    char* cmd;
#ifdef WIN32
    char* context = NULL;
#else
    home = (char*)calloc(200, sizeof(char));
#endif
    filename = (char*)calloc(200, sizeof(char));
    syncdir = (char*)calloc(200, sizeof(char));
    if (prelude() == -1) return -1;
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
            sprintf_s(syncdir, 200, "%s", sqlite3_column_text(stmt, 1));
#else
            sprintf(syncdir, "%s", sqlite3_column_text(stmt, 1));
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
        else if (strcmp((const char*)sqlite3_column_text(stmt, 0), "5") == 0) {
            vv = atoi((const char*)sqlite3_column_text(stmt, 1));
            }
        if (strcmp((const char*)sqlite3_column_text(stmt, 0), "15") == 0) {
#ifdef WIN32
            sprintf_s(filename, 200, "%s/%s", syncdir, sqlite3_column_text(stmt, 1));
#else
            sprintf(filename, "%s/%s", syncdir, sqlite3_column_text(stmt, 1));
#endif
            }
#ifndef WIN32
        else if (strcmp((const char*)sqlite3_column_text(stmt, 0), "20") == 0) {
            sprintf(home, "HOME=%s", sqlite3_column_text(stmt, 1));
            }
#endif
        }
    if (git == 1 || hg == 1 || svn == 1 || vv == 1) {
        cmd = (char*)calloc(200, sizeof(char));
#ifndef WIN32
        putenv(home);
#endif
        if (git == 1) {
#ifdef WIN32
            sprintf_s(cmd, 200, "cd %s;git pull", syncdir);
#else
            sprintf(cmd, "cd %s;git pull", syncdir);
#endif
            }
        else if (hg == 1) {
#ifdef WIN32
            sprintf_s(cmd, 200, "cd %s;hg pull --update", syncdir);
#else
            sprintf(cmd, "cd %s;hg pull --update", syncdir);
#endif
            }
        else if (svn == 1) {
#ifdef WIN32
            sprintf_s(cmd, 200, "cd %s;svn update", syncdir);
#else
            sprintf(cmd, "cd %s;svn update", syncdir);
#endif
            }
        else if (vv == 1) {
#ifdef WIN32
            sprintf_s(cmd, 200, "cd %s;vv pull", syncdir);
#else
            sprintf(cmd, "cd %s;vv pull", syncdir);
#endif
            }
        if (system(cmd)) return -1;
        }
    printf("Sync file: %s\n\r", filename);
#ifdef WIN32
    fopen_s(&f, filename, "a+");
#else
    f = fopen(filename, "a+");
#endif
    if (f == NULL) {
        printf("There is no such file and it's failed to create it\n\r");
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
        if (git == 1 || hg == 1 || svn == 1 || vv == 1) {
#ifndef WIN32
            putenv(home);
#endif
            if (git == 1) {
#ifdef WIN32
                sprintf_s(cmd, 200, "cd %s;git commit -am \"TODO LIST UPDATE\";git push", syncdir);
#else
                sprintf(cmd, "cd %s;git commit -am \"TODO LIST UPDATE\";git push", syncdir);
#endif
                }
            else if (hg == 1) {
#ifdef WIN32
                sprintf_s(cmd, 200, "cd %s;hg commit -m \"TODO LIST UPDATE\";hg push", syncdir);
#else
                sprintf(cmd, "cd %s;hg commit -m \"TODO LIST UPDATE\";hg push", syncdir);
#endif
                }
            else if (svn == 1) {
#ifdef WIN32
                sprintf_s(cmd, 200, "cd %s;svn commit  -m \"TODO LIST UPDATE\"", syncdir);
#else
                sprintf(cmd, "cd %s;svn commit  -m \"TODO LIST UPDATE\"", syncdir);
#endif
                }
            else if (vv == 1) {
#ifdef WIN32
                sprintf_s(cmd, 200, "cd %s;vv commit -m \"TODO LIST UPDATE\";vv push", syncdir);
#else
                sprintf(cmd, "cd %s;vv commit -m \"TODO LIST UPDATE\"; vv push", syncdir);
#endif
                }
            if (system(cmd) == -1) return -1;
            }
        printf("synchronization complete, syncfile updated\n\r");
        }
    else {
        timeUpdate(timefile);
        printf("synchronization complete, local database updated\n\r");
        }
#ifndef WIN32
    free(home);
#endif
    free(syncdir);
    free(filename);
    return 0;
    }
void todo_edit(char** argv, int argc) {
    int argi;
    char* text = (char*)calloc(200, sizeof(char));
    if (prelude() != -1) {
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
void todo_swap(char** argv) {
    int val1 = atoi(argv[2]);
    int val2 = atoi(argv[3]);
    if (prelude() != -1) {
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
//________________________________________________________________________________
void todo_mv(char** argv) {
    if (prelude() != -1) {
#ifdef WIN32
        sprintf_s(queries[ind++], 255, "UPDATE TODO SET id = %s WHERE id = %s", argv[3], argv[2]);
#else
        sprintf(queries[ind++], "UPDATE TODO SET id = %s WHERE id = %s", argv[3], argv[2]);
#endif
        retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
        timeUpdate(time(0));
        }
    }
//________________________________________________________________________________
void todo_clean() {
    char answer;
    if (prelude() != -1) {
        printf("Are you sure that you want to clean all the tasks? (y/n)");
#ifdef WIN32
        if (scanf_s("%c", &answer) > 0) {
#else
        if (scanf("%c", &answer) > 0) {
#endif
            if (answer == 'y') {
                queries[ind++] = "DELETE FROM TODO";
                retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
                timeUpdate(time(0));
                }
            }
        }
    }
//________________________________________________________________________________
void todo_rm(char** argv) {
    if (prelude() != -1) {
#ifdef WIN32
        sprintf_s(queries[ind++], 255, "DELETE FROM TODO WHERE id = %s", argv[2]);
#else
        sprintf(queries[ind++], "DELETE FROM TODO WHERE id = %s", argv[2]);
#endif
        retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
        timeUpdate(time(0));
        }
    }
//________________________________________________________________________________
char** todo_read(int index, int parcount) {
    char* lineborder1;
    char* spaces1;
    char* spaces2;
    int maxl2 = 0, maxl1 = 0;
    int i, maxi1, maxi2;
    if (prelude() == -1) return NULL;
    out = (char**)malloc(sizeof(char*) * 100);
    for (x = 0; x < 100; x++) {
        out[x] = (char*)malloc(sizeof(char) * 255);
        }
#ifndef WIN32
    char* colorscheme;
    char* lineborder2;
    ///<Summary>
    ///Get color scheme (For linux only)
    ///</Summary>
    sprintf(queries[ind++], "SELECT option, text FROM OPTIONS WHERE option = 21");
    retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
    if (retval) {
        printf("Failed to get color scheme\n\r");
        }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (strcmp((const char*)sqlite3_column_text(stmt, 0), "21") == 0) {
            colorscheme = (char*)calloc(50, sizeof(char));
            if (strcmp((const char*)sqlite3_column_text(stmt, 1), "red") == 0) {
                sprintf(colorscheme, "%c[%d;%d;%dm", 0x1B, 1, 37, 41);
                }
            else if (strcmp((const char*)sqlite3_column_text(stmt, 1), "blink") == 0) {
                sprintf(colorscheme, "%c[%d;%d;%dm", 0x1B, 1, 50, 5);
                }
            else if (strcmp((const char*)sqlite3_column_text(stmt, 1), "green") == 0) {
                sprintf(colorscheme, "%c[%d;%d;%dm", 0x1B, 1, 68, 32);
                }
            else if (strcmp((const char*)sqlite3_column_text(stmt, 1), "pink") == 0) {
                sprintf(colorscheme, "%c[%d;%d;%dm", 0x1B, 1, 35, 2);
                }
            else if (strcmp((const char*)sqlite3_column_text(stmt, 1), "black") == 0) {
                sprintf(colorscheme, "%c[%d;%d;%dm", 0x1B, 1, 6, 66);
                }
            else {
                sprintf(colorscheme, "%c[%d;%d;%dm", 0x1B, 1, 37, 41);
                }
            break;
            }
        }
#endif
    if (parcount > 0) {
#ifdef WIN32
        sprintf_s(queries[ind++], 255, "SELECT COALESCE(MAX(id),0) FROM TODO WHERE id = %d", index);
#else
        sprintf(queries[ind++], "SELECT COALESCE(MAX(id),0) FROM TODO WHERE id = %d", index);
#endif
        }
    else queries[ind++] = "SELECT COALESCE(MAX(id),0) from TODO";
    retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
    if (retval) {
        printf("Reading data from DB Failed, run initdb first\n\r");
        return NULL;
        }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        maxl1 = strlen((const char*)sqlite3_column_text(stmt, 0));
        }
    if (parcount > 0) {
#ifdef WIN32
        sprintf_s(queries[ind++], 255, "SELECT COALESCE(MAX(LENGTH(text)),0) FROM TODO WHERE id = %d", index);
#else
        sprintf(queries[ind++], "SELECT COALESCE(MAX(LENGTH(text)),0) FROM TODO WHERE id = %d", index);
#endif
        }
    else queries[ind++] = "SELECT COALESCE(MAX(LENGTH(text)),0) from TODO";
    retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
    if (retval) {
        printf("Reading data from DB Failed, run initdb first\n\r");
        return NULL;
        }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        maxl2 = atoi((const char*)sqlite3_column_text(stmt, 0));
        }
    if (parcount > 0) {
#ifdef WIN32
        sprintf_s(queries[ind++], 255, "SELECT id, text, LENGTH(text) FROM TODO WHERE id = %d", index);
#else
        sprintf(queries[ind++], "SELECT id, text, LENGTH(text) FROM TODO WHERE id = %d", index);
#endif
        }
    else queries[ind++] = "SELECT id, text, LENGTH(text) from TODO";
    retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
    if (retval) {
        printf("Selecting data from DB Failed, run initdb first\n\r");
        return NULL;
        }
    lineborder1 = (char*)calloc(255, sizeof(char));
#ifndef WIN32
    lineborder2 = (char*)calloc(255, sizeof(char));
#endif
    spaces1    = (char*)calloc(200, sizeof(char));
    spaces2    = (char*)calloc(200, sizeof(char));
    for (i = 0; i < ((maxl2 + maxl1) + 5); i++) {
        if (i == 2 + maxl1) {
#ifdef WIN32
            strcat_s(lineborder1, 200, "+");
#else
            strcat(lineborder1, "╤");
            strcat(lineborder2, "╧");
#endif
            }
        else {
#ifdef WIN32
            strcat_s(lineborder1, 200, "-");
#else
            strcat(lineborder1, "═");
            strcat(lineborder2, "═");
#endif
            }
        }
#ifdef WIN32
    sprintf_s(out[0], 255, "%s", lineborder1);
    x = 2;
#else
    sprintf(out[1], "%s", colorscheme);
    sprintf(out[0], "%s", lineborder1);
    x = 4;
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
        sprintf_s(out[x], 255, "| %s %s| %s %s|\n\r",
               sqlite3_column_text(stmt, 0)
               , spaces1
               , sqlite3_column_text(stmt, 1)
               , spaces2);
        x+=1;
#else
        sprintf(out[x], " %s %s",
               sqlite3_column_text(stmt, 0)
               , spaces1);
        sprintf(out[x+1], " %s %s"
               , sqlite3_column_text(stmt, 1)
               , spaces2);
        x+=2;
#endif
        }
#ifdef WIN32
    memcpy(out[1], &x, sizeof(int));
#else
    memcpy(out[3], &x, sizeof(int));
    sprintf(out[2], "%s", colorscheme);
    sprintf(out[1], "%s", lineborder2);
#endif
    free(lineborder1);
    free(spaces1);
    free(spaces2);
#ifndef WIN32
    free(lineborder2);
    free(colorscheme);
#endif
    close();
    return out;
    }
//________________________________________________________________________________
int todo_write(char** argv, int argc) {
    char first = 0;
    int counter;
    int last = 0;
    int argi;
    char* text;
    char* ending;
    int useending = 0;
    int limit = 200;
    if (prelude() == -1) return -1;
    ///<Summary>
    ///Getting options from local database
    ///<Summary>
#ifdef WIN32
    sprintf_s(queries[ind++], 255, "SELECT option, text FROM OPTIONS WHERE option = 12 or option = 13");
#else
    sprintf(queries[ind++], "SELECT option, text FROM OPTIONS WHERE option = 12 or option = 13");
#endif
    retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
    if (retval) {
        printf("Sync data Failed, run initdb first\n\r");
        return -1;
        }
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        if (strcmp((const char*)sqlite3_column_text(stmt, 0), "13") == 0) {
            ending = (char*)calloc(200, sizeof(char));
#ifdef WIN32
            sprintf_s(ending, 200, "%s", sqlite3_column_text(stmt, 1));
#else
            sprintf(ending, "%s", sqlite3_column_text(stmt, 1));
#endif
            }
        else if (strcmp((const char*)sqlite3_column_text(stmt, 0), "12") == 0) {
            useending = atoi((const char*)sqlite3_column_text(stmt, 1));
            }
        }
    ///<Summary>
    ///Writing to local database
    ///<Summary>
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
    text = (char*)calloc(200, sizeof(char));
    if (useending == 1) {
        limit = 200 - strlen(ending);
        }
    for (argi = 1; argi < argc; argi++) {
        if (strlen(text) + strlen(argv[argi]) + sizeof(char)  >= (unsigned int)limit) {
            break;
            }
        else {
            if ((strcmp(argv[argi], "--motivate") == 0)) {
                useending = 1;
                }
            else if (strcmp(argv[argi], "--first") == 0) {
                first = 1;
                }
            else {
#ifdef WIN32
                strcat_s(text, 200, argv[argi]);
                strcat_s(text, 200, " ");
#else
                strcat(text, argv[argi]);
                strcat(text, " ");
#endif
                }
            }
        }
    if (useending == 1) {
#ifdef WIN32
        strcat_s(text, 200, ending);
#else
        strcat(text, ending);
#endif
        }
    if (first == 1) {
        for (counter = 1; counter < last; counter++) {
            sql("UPDATE TODO SET id = id + 1000000000");
            sql("UPDATE TODO SET id = id - (1000000000 - 1)");
            }
#ifdef WIN32
        sprintf_s(queries[ind++], 255, "INSERT INTO TODO VALUES(0,'%s')", text);
#else
        sprintf(queries[ind++], "INSERT INTO TODO VALUES(0,'%s')", text);
#endif
        }
    else {
#ifdef WIN32
        sprintf_s(queries[ind++], 255, "INSERT INTO TODO VALUES(%d,'%s')", last + 1, text);
#else
        sprintf(queries[ind++], "INSERT INTO TODO VALUES(%d,'%s')", last + 1, text);
#endif
        }
    retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
    if (retval) {
        printf("Task were not added! (shit happens)\n\r");
        return -1;
        }
    free(text);
    free(ending);
    timeUpdate(time(0));
    return 0;
    }
//________________________________________________________________________________
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
