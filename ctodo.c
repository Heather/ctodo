#define Console

#ifdef _MSC_VER
#include "sqlite3.h"
#else
#include <sqlite3.h>
#endif
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

///General properties
int retval, x, ind = 0;
char* dest;
char** out;

///Pointers
FILE* f;
time_t timefile;
sqlite3_stmt* stmt;

///Maximum sqlite query
int q_size = 150;
#ifdef WIN
int q_cnt = 26;
#else
char* home;
int q_cnt = 30;
#endif

///Sqlite handlers
char** queries;
sqlite3* handle;

char* todo_version() {
  return "  CTODO List Management Uti v2.1.6\n";
}

char* todo_help() {
  dest = (char*)calloc(4000, sizeof(char));
#ifdef _MSC_VER
  strcpy_s(dest, 4000, todo_version());
#else
  strcpy(dest, todo_version());
#endif
#ifdef _MSC_VER
  strcat_s(dest, 4000,
#else
  strcat(dest,
#endif
"    Cross-platform todo list management library\n\
    Copyright (C) 2012-2018 Cynede\n");
  return &dest[0];
}
#ifdef Console
void shitHappended() {
    printf("Shit happened, try to resolve it by yourself :(\n\r");
}
#endif

void timeUpdate(time_t t) {
#ifdef _MSC_VER
  sprintf_s(queries[ind++], q_size, "INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (1,'%d')", (int)t);
#else
  sprintf(queries[ind++], "INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (1,'%d')", (int)t);
#endif
  retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
  if (retval) {
#ifdef Console
    printf("Failed to update db time\n\r");
#endif
    return;
  }
}

void sql(char* command) {
#ifdef _MSC_VER
  sprintf_s(queries[ind++], q_size, "%s", command);
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

///TODO: clean allocated space
int prelude() {
  timefile = 0;
  f = NULL;
#ifndef WIN32
  char* temp = (char*)calloc(200, sizeof(char));
#endif
  queries = (char**)malloc(sizeof(char*)* q_cnt);
  for (x = 0; x < q_cnt; x++)
    queries[x] = (char*)malloc(sizeof(char)* q_size);
#ifdef WIN32
  retval = sqlite3_open("todo.db3", &handle);
#else
  home = (char*)getenv("HOME");
  strcpy(temp, home);
  retval = sqlite3_open(strcat(temp, "/.todo.db3"), &handle);
  free(temp);
#endif
  if (retval) {
#ifdef Console
    printf("Database connection failed\n\r");
#endif
    return -1;
  }
  return 0;
}
int prelude_custom(char* db) {
  timefile = 0;
  f = NULL;
  queries = (char**)malloc(sizeof(char*)* q_cnt);
  for (x = 0; x < q_cnt; x++)
    queries[x] = (char*)malloc(sizeof(char)* q_size);
  retval = sqlite3_open(db, &handle);
  if (retval) {
#ifdef Console
    printf("Database connection failed\n\r");
#endif
    return -1;
  }
  return 0;
}

void todo_close() {
  free(queries);
  free(dest);
#ifndef _MSC_VER
  free(home);
#endif
  free(out);
  sqlite3_close(handle);
}

int todo_initdb_meta() {
  sql("CREATE TABLE IF NOT EXISTS TODO (id INTEGER PRIMARY KEY,text TEXT NOT NULL, list INTEGER NOT NULL)");
  if (retval) {
#ifdef Console
    printf("Init DB Failed, Shit happens?\n\r");
#endif
    return -1;
  }
  sql("CREATE TABLE IF NOT EXISTS OPTIONS (option INTEGER PRIMARY KEY,text TEXT NOT NULL)");
  sql("CREATE TABLE IF NOT EXISTS NAMELIST (option INTEGER PRIMARY KEY,name TEXT NOT NULL)");
  ///<Option>
  ///Sync file for tex serialization
  ///</Option>
  sql("INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (15,'readme.md')");
  ///<Option>
  ///Time of last synchronization
  ///</Option>
#ifdef _MSC_VER
  sprintf_s(queries[ind++], q_size, "INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (1,'%d')", 0);
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
  ///Use darcs (darcs) for synchronization
  ///</Option>
  sql("INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (5'0')");
  ///<Option>
  ///Add ending word to each todo row
  ///</Option>
  sql("INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (12,'0')");
  ///<Option>
  ///Ending word (--motivate)
  ///</Option>
  sql("INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (13,'do it')");
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
  sql("INSERT OR REPLACE INTO OPTIONS (option,text) VALUES (21,'black')");
  if (retval) {
#ifdef Console
    printf("Instert deafaults options Failed, Shit happens?\n\r");
#endif
    return -1;
  }
#endif
  return 0;
}
int todo_initdb() {
  if (prelude() == -1) return -1;
  return todo_initdb_meta();
}
int todo_initdb_custom(char* db) {
  if (prelude_custom(db) == -1) return -1;
  return todo_initdb_meta();
}

int todo_set_meta(char** argv, int argc) {
  int opt = 0;
  if (argc < 4) {
#ifdef Console
    printf("set what?\n\r");
#endif
  } else if (strcmp(argv[2], "syncfile") == 0)    opt = 15;
  else if (strcmp(argv[2], "syncdir") == 0)       opt = 0;
  else if (strcmp(argv[2], "ending") == 0)        opt = 13;
  else if (strcmp(argv[2], "color") == 0)         opt = 21;
  else if (strcmp(argv[2], "end") == 0)
  if ((strcmp(argv[3], "1") == 0) || (strcmp(argv[3], "0") == 0))
    opt = 12;
  else {
#ifdef Console
    printf("Use 1 or 0 for this option\n\r");
#endif
  } else if ((strcmp(argv[2], "git") == 0)
    || (strcmp(argv[2], "hg") == 0)
    || (strcmp(argv[2], "svn") == 0)
    || (strcmp(argv[2], "darcs") == 0)) {
    if ((strcmp(argv[3], "1") == 0) || (strcmp(argv[3], "0") == 0)) {
      if (strcmp(argv[2], "git") == 0)            opt = 2;
      else if (strcmp(argv[2], "hg") == 0)        opt = 3;
      else if (strcmp(argv[2], "svn") == 0)       opt = 4;
      else if (strcmp(argv[2], "darcs") == 0)     opt = 5;
    } else {
#ifdef Console
      printf("Use 1 or 0 for this option\n\r");
#endif
    }
  }
#ifndef _MSC_VER
  else if (strcmp(argv[2], "home") == 0) {
    sprintf(queries[ind++], "UPDATE OPTIONS SET text='%s' WHERE option = 20", argv[3]);
    retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
    if (retval) {
#ifdef Console
      printf("Option home is not changed! (shit happens)\n\r");
#endif
      return -1;
    }   }
#endif
  else {
#ifdef Console
    printf("There is no such option\n\r");
#endif
    return 0;
  }
#ifdef _MSC_VER
  sprintf_s(queries[ind++], q_size, "UPDATE OPTIONS SET text='%s' WHERE option = %d", argv[3], opt);
#else
  sprintf(queries[ind++], "UPDATE OPTIONS SET text='%s' WHERE option = %d", argv[3], opt);
#endif
  retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
  if (retval) {
#ifdef Console
    printf("Option is not changed! (shit happens)\n\r");
#endif
    return -1;
  }
  return 0;
}
int todo_set(char** argv, int argc) {
  if (prelude() == -1) return -1;
  else return todo_set_meta(argv, argc);
}
int todo_set_custom(char** argv, int argc, char* db) {
  if (prelude_custom(db) == -1) return -1;
  else return todo_set_meta(argv, argc);
}

int todo_show_meta(char** argv, int argc) {
  char* opt = (char*)calloc(3, sizeof(char));
  char* answer = (char*)calloc(200, sizeof(char));
  if (argc < 3) {
#ifdef Console
    printf("show what?\n\r");
#endif
  } else if (strcmp(argv[2], "syncfile") == 0) sprintf(opt, "15");
  else if (strcmp(argv[2], "syncdir") == 0)    sprintf(opt, "0");
  else if (strcmp(argv[2], "ending") == 0)     sprintf(opt, "13");
  else if (strcmp(argv[2], "color") == 0)      sprintf(opt, "21");
  #ifdef _MSC_VER
    sprintf_s(queries[ind++], q_size, "SELECT option, text FROM OPTIONS");
  #else
    sprintf(queries[ind++], "SELECT option, text FROM OPTIONS");
  #endif
  retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
  if (retval) {
#ifdef Console
    ///TODO
    shitHappended();
#endif
      return -1;
  }
  while (sqlite3_step(stmt) == SQLITE_ROW)
  if (strcmp((const char*)sqlite3_column_text(stmt, 0), opt) == 0) {
    #ifdef _MSC_VER
        printf_s(": %s", sqlite3_column_text(stmt, 1));
    #else
        printf(": %s", sqlite3_column_text(stmt, 1));
    #endif
  }
  free(opt);
  free(answer);
  return 0;
}
int todo_show(char** argv, int argc) {
  if (prelude() == -1) return -1;
  else return todo_show_meta(argv, argc);
}
int todo_show_custom(char** argv, int argc, char* db) {
  if (prelude_custom(db) == -1) return -1;
  else return todo_show_meta(argv, argc);
}

int todo_history_meta() {
  char* syncdir;
  char* cmd = (char*)calloc(200, sizeof(char));
  int git = 0, hg = 0, svn = 0;
#ifdef _MSC_VER
  sprintf_s(queries[ind++], q_size, "SELECT option, text FROM OPTIONS");
#else
  sprintf(queries[ind++], "SELECT option, text FROM OPTIONS");
#endif
  retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
  if (retval) {
#ifdef Console
    printf("Reading DB data Failed, running re-init\n\r");
#endif
    if (todo_initdb() == 0) {
#ifdef Console
      printf("Done\n\r");
#endif
    } else {
#ifdef Console
      shitHappended();
#endif
      return -1;
    }
    return todo_history_meta();
  }
  syncdir = (char*)calloc(200, sizeof(char));
  while (sqlite3_step(stmt) == SQLITE_ROW)
  if (strcmp((const char*)sqlite3_column_text(stmt, 0), "0") == 0)
#ifdef _MSC_VER
    sprintf_s(syncdir, 200, "%s", sqlite3_column_text(stmt, 1));
#else
    sprintf(syncdir, "%s", sqlite3_column_text(stmt, 1));
#endif
  else if (strcmp((const char*)sqlite3_column_text(stmt, 0), "2") == 0)
    git = atoi((const char*)sqlite3_column_text(stmt, 1));
  else if (strcmp((const char*)sqlite3_column_text(stmt, 0), "3") == 0)
    hg = atoi((const char*)sqlite3_column_text(stmt, 1));
  else if (strcmp((const char*)sqlite3_column_text(stmt, 0), "4") == 0)
    svn = atoi((const char*)sqlite3_column_text(stmt, 1));
  if (git == 1 || hg == 1 || svn == 1) {
#ifndef _MSC_VER
    putenv(home);
#endif
    if (git == 1)
#ifdef _MSC_VER
      sprintf_s(cmd, 200, "cd %s;git log", syncdir);
#else
      sprintf(cmd, "cd %s;git log", syncdir);
#endif
    else if (hg == 1)
#ifdef _MSC_VER
      sprintf_s(cmd, 200, "cd %s;hg log", syncdir);
#else
      sprintf(cmd, "cd %s;hg log", syncdir);
#endif
    else if (svn == 1)
#ifdef _MSC_VER
      sprintf_s(cmd, 200, "cd %s;svn log", syncdir);
#else
      sprintf(cmd, "cd %s;svn log", syncdir);
#endif
    if (system(cmd) == -1) return -1;
  }
  free(syncdir);
  free(cmd);
  return 0;
}
int todo_history() {
  if (prelude() == -1) return -1;
  else return todo_history_meta();
}
int todo_history_custom(char* db) {
  if (prelude_custom(db) == -1) return -1;
  else return todo_history_meta();
}

int todo_sync_meta(char** argv) {
  char* filename;
#ifndef _MSC_VER
  char* home;
#endif
  int git = 0, hg = 0, svn = 0, darcs = 0;
  int timeDB = 0;
  int i = 0;
  char line[150];
  char write = 1;
  char* token1;
  char* token2;
  char* search = "|";
  char* syncdir;
  char* cmd = (char*)calloc(200, sizeof(char));
#ifdef _MSC_VER
  char* context = NULL;
#else
  home = (char*)calloc(200, sizeof(char));
#endif
  filename = (char*)calloc(200, sizeof(char));
  syncdir = (char*)calloc(200, sizeof(char));
#ifdef _MSC_VER
  sprintf_s(queries[ind++], q_size, "SELECT option, text FROM OPTIONS");
#else
  sprintf(queries[ind++], "SELECT option, text FROM OPTIONS");
#endif
  retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
  if (retval) {
#ifdef Console
    printf("Reading DB data Failed, running re-init\n\r");
#endif
    if (todo_initdb() == 0) {
#ifdef Console
      printf("Done\n\r");
#endif
    } else {
#ifdef Console
      shitHappended();
#endif
      return -1;
    }
    return todo_sync_meta(argv);
  }
  while (sqlite3_step(stmt) == SQLITE_ROW)
  if (strcmp((const char*)sqlite3_column_text(stmt, 0), "0") == 0)
#ifdef _MSC_VER
    sprintf_s(syncdir, 200, "%s", sqlite3_column_text(stmt, 1));
#else
    sprintf(syncdir, "%s", sqlite3_column_text(stmt, 1));
#endif
  else if (strcmp((const char*)sqlite3_column_text(stmt, 0), "1") == 0)
    timeDB = atoi((const char*)sqlite3_column_text(stmt, 1));
  else if (strcmp((const char*)sqlite3_column_text(stmt, 0), "2") == 0)
    git = atoi((const char*)sqlite3_column_text(stmt, 1));
  else if (strcmp((const char*)sqlite3_column_text(stmt, 0), "3") == 0)
    hg = atoi((const char*)sqlite3_column_text(stmt, 1));
  else if (strcmp((const char*)sqlite3_column_text(stmt, 0), "4") == 0)
    svn = atoi((const char*)sqlite3_column_text(stmt, 1));
  else if (strcmp((const char*)sqlite3_column_text(stmt, 0), "5") == 0)
    darcs = atoi((const char*)sqlite3_column_text(stmt, 1));
  else if (strcmp((const char*)sqlite3_column_text(stmt, 0), "15") == 0)
#ifdef _MSC_VER
    sprintf_s(filename, 200, "%s/%s", syncdir, sqlite3_column_text(stmt, 1));
#else
    sprintf(filename, "%s/%s", syncdir, sqlite3_column_text(stmt, 1));
#endif
#ifndef _MSC_VER
  else if (strcmp((const char*)sqlite3_column_text(stmt, 0), "20") == 0)
    sprintf(home, "HOME=%s", sqlite3_column_text(stmt, 1));
#endif
  if (git == 1 || hg == 1 || svn == 1 || darcs == 1) {
#ifndef _MSC_VER
    putenv(home);
#endif
    if (git == 1)
#ifdef _MSC_VER
      sprintf_s(cmd, 200, "cd %s;git pull", syncdir);
#else
      sprintf(cmd, "cd %s;git pull", syncdir);
#endif
    else if (hg == 1)
#ifdef _MSC_VER
      sprintf_s(cmd, 200, "cd %s;hg pull --update", syncdir);
#else
      sprintf(cmd, "cd %s;hg pull --update", syncdir);
#endif
    else if (svn == 1)
#ifdef _MSC_VER
      sprintf_s(cmd, 200, "cd %s;svn update", syncdir);
#else
      sprintf(cmd, "cd %s;svn update", syncdir);
#endif
    else if (darcs == 1)
#ifdef _MSC_VER
      sprintf_s(cmd, 200, "cd %s;darcs pull", syncdir);
#else
      sprintf(cmd, "cd %s;darcs pull", syncdir);
#endif
    if (system(cmd)) return -1;
  }
#ifdef Console
  printf("Sync file: %s\n\r", filename);
#endif
#ifdef _MSC_VER
  fopen_s(&f, filename, "a+");
#else
  f = fopen(filename, "a+");
#endif
  if (f == NULL) {
#ifdef Console
    printf("There is no such file and it's failed to create it\n\r");
#endif
    return -1;
  }
  while (fgets(line, 150, f)) {
    if (i == 0) {
      timefile = atoi(line);
#ifndef _MSC_VER
#ifdef Console
      printf("Timefile: %s\n\r", ctime(&timefile));
#endif
#endif
      if (timeDB > (int)timefile) break;
      else if (timeDB == (int)timefile) {
#ifdef Console
        printf("Everything is up to date\n\r");
#endif
        return 0;
      } else write = 0;
    } else {
      if (i == 1) {
        queries[ind++] = "DELETE FROM TODO";
        retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
        if (retval) {
#ifdef Console
          printf("Reading DB data Failed, running re-init\n\r");
#endif
          if (todo_initdb() == 0) {
#ifdef Console
            printf("Done\n\r");
#endif
          } else {
#ifdef Console
            shitHappended();
#endif
            return -1;
          }
          return todo_sync_meta(argv);
        }
      } else {
#ifdef _MSC_VER
        token1 = strtok_s(line, search, &context);
        token2 = strtok_s(NULL, search, &context);
#else
        token1 = strtok(line, search);
        token2 = strtok(NULL, search);
#endif
        if (token1[1] == '-') token1 += 3;
        rtrim(token2);
#ifdef _MSC_VER
        sprintf_s(queries[ind++], q_size, "INSERT INTO TODO VALUES(%s,'%s', 0)", token1, token2);
#else
        sprintf(queries[ind++], "INSERT INTO TODO VALUES(%s,'%s', 0)", token1, token2);
#endif
        retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
        if (retval) {
#ifdef Console
          printf("Task were not added! (shit happens)\n\r");
#endif
        }
      }
    }
    i++;
  }
  fclose(f);
  if (write) {
    time_t now = time(0);
#ifdef _MSC_VER
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
    while (sqlite3_step(stmt) == SQLITE_ROW)
      fprintf(f, " - %s|%s\n"
      , sqlite3_column_text(stmt, 0)
      , sqlite3_column_text(stmt, 1));
    fclose(f);
    if (git == 1 || hg == 1 || svn == 1 || darcs == 1) {
#ifndef _MSC_VER
      putenv(home);
#endif
      if (git == 1)
#ifdef _MSC_VER
        sprintf_s(cmd, 200, "cd %s;git commit -am \"TODO LIST UPDATE\";git push", syncdir);
#else
        sprintf(cmd, "cd %s;git commit -am \"TODO LIST UPDATE\";git push", syncdir);
#endif
      else if (hg == 1)
#ifdef _MSC_VER
        sprintf_s(cmd, 200, "cd %s;hg commit -m \"TODO LIST UPDATE\";hg push", syncdir);
#else
        sprintf(cmd, "cd %s;hg commit -m \"TODO LIST UPDATE\";hg push", syncdir);
#endif
      else if (svn == 1)
#ifdef _MSC_VER
        sprintf_s(cmd, 200, "cd %s;svn commit  -m \"TODO LIST UPDATE\"", syncdir);
#else
        sprintf(cmd, "cd %s;svn commit  -m \"TODO LIST UPDATE\"", syncdir);
#endif
      else if (darcs == 1)
#ifdef _MSC_VER
        sprintf_s(cmd, 200, "cd %s;darcs commit -m \"TODO LIST UPDATE\";darcs push", syncdir);
#else
        sprintf(cmd, "cd %s;darcs commit -m \"TODO LIST UPDATE\"; darcs push", syncdir);
#endif
      if (system(cmd) == -1) return -1;
    }
#ifdef Console
    printf("synchronization complete, syncfile updated\n\r");
#endif
  } else {
    timeUpdate(timefile);
#ifdef Console
    printf("synchronization complete, local database updated\n\r");
#endif
  }
  free(syncdir);
  free(filename);
  return 0;
}
int todo_sync(char** argv) {
  if (prelude() == -1) return -1;
  return todo_sync_meta(argv);
}
int todo_sync_custom(char** argv, char* db) {
  if (prelude_custom(db) == -1) return -1;
  return todo_sync_meta(argv);
}

void todo_edit_meta(char** argv, int argc) {
  int argi;
  char* text = (char*)calloc(200, sizeof(char));
  for (argi = 3; argi < argc; argi++) {
#ifdef _MSC_VER
    strcat_s(text, 200, argv[argi]);
    strcat_s(text, 200, " ");
#else
    strcat(text, argv[argi]);
    strcat(text, " ");
#endif
  }
#ifdef _MSC_VER
  sprintf_s(queries[ind++], q_size, "UPDATE TODO SET text='%s' WHERE id = %s", text, argv[2]);
#else
  sprintf(queries[ind++], "UPDATE TODO SET text='%s' WHERE id = %s", text, argv[2]);
#endif
  retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
  if (retval) {
#ifdef Console
    printf("Task were not edited! (shit happens)\n\r");
#endif
  }
  free(text);
  timeUpdate(time(0));
}
void todo_edit(char** argv, int argc) {
  if (prelude() != -1) todo_edit_meta(argv, argc);
}
void todo_edit_custom(char** argv, int argc, char* db) {
  if (prelude_custom(db) != -1) todo_edit_meta(argv, argc);
}

void todo_swap_meta(char** argv) {
  int val1 = atoi(argv[2]);
  int val2 = atoi(argv[3]);
#ifdef _MSC_VER
  sprintf_s(queries[ind++], q_size, "UPDATE TODO SET id=%d WHERE id = %d", 9999, val1);
#else
  sprintf(queries[ind++], "UPDATE TODO SET id=%d WHERE id = %d", 9999, val1);
#endif
  retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
  if (retval) {
#ifdef Console
    printf("Swap failed! (shit happens)\n\r");
#endif
  }
#ifdef _MSC_VER
  sprintf_s(queries[ind++], q_size, "UPDATE TODO SET id=%d WHERE id = %d", val1, val2);
#else
  sprintf(queries[ind++], "UPDATE TODO SET id=%d WHERE id = %d", val1, val2);
#endif
  retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
  if (retval) {
#ifdef Console
    printf("Swap failed! (shit happens)\n\r");
#endif
  }
#ifdef _MSC_VER
  sprintf_s(queries[ind++], q_size, "UPDATE TODO SET id=%d WHERE id = %d", val2, 9999);
#else
  sprintf(queries[ind++], "UPDATE TODO SET id=%d WHERE id = %d", val2, 9999);
#endif
  retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
  if (retval) {
#ifdef Console
    printf("Swap failed! (shit happens)\n\r");
#endif
  }
  timeUpdate(time(0));
}
void todo_swap(char** argv) {
  if (prelude() != -1) todo_swap_meta(argv);
}
void todo_swap_custom(char** argv, char* db) {
  if (prelude_custom(db) != -1) todo_swap_meta(argv);
}

void todo_reindex() {
  if (prelude() != -1) {
    sql("UPDATE TODO SET id = id + 1000000000");
    sql("UPDATE TODO SET id = rowid");
    timeUpdate(time(0));
  }
}

void todo_mv_meta(char** argv) {
#ifdef _MSC_VER
  sprintf_s(queries[ind++], q_size, "UPDATE TODO SET id = %s WHERE id = %s", argv[3], argv[2]);
#else
  sprintf(queries[ind++], "UPDATE TODO SET id = %s WHERE id = %s", argv[3], argv[2]);
#endif
  retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
  timeUpdate(time(0));
}
void todo_mv(char** argv) {
  if (prelude() != -1) todo_mv_meta(argv);
}
void todo_mv_custom(char** argv, char* db) {
  if (prelude_custom(db) != -1) todo_mv_meta(argv);
}

void todo_clean_meta() {
  queries[ind++] = "DELETE FROM TODO";
  retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
  timeUpdate(time(0));
}
void todo_clean() {
  if (prelude() != -1) todo_clean_meta();
}
void todo_clean_custom(char* db) {
  if (prelude_custom(db) != -1) todo_clean_meta();
}

void todo_rm_meta(char** argv) {
    if (strstr(argv[2], ",") != NULL) {
#ifdef _MSC_VER
  sprintf_s(queries[ind++], q_size, "DELETE FROM TODO WHERE id IN (%s)", argv[2]);
#else
  sprintf(queries[ind++], "DELETE FROM TODO WHERE id IN (%s)", argv[2]);
#endif
    } else {
#ifdef _MSC_VER
  sprintf_s(queries[ind++], q_size, "DELETE FROM TODO WHERE id = %s", argv[2]);
#else
  sprintf(queries[ind++], "DELETE FROM TODO WHERE id = %s", argv[2]);
#endif
    }
  retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
  timeUpdate(time(0));
}
void todo_rm(char** argv) {
  if (prelude() != -1) todo_rm_meta(argv);
}
void todo_rm_custom(char** argv, char* db) {
  if (prelude_custom(db) != -1) todo_rm_meta(argv);
}

char** todo_read_meta(int list, int parcount) {
  char* lineborder1;
  char* spaces1;
  char* spaces2;
  int maxl2 = 0, maxl1 = 0;
  int i, maxi1, maxi2;
  out = (char**)malloc(sizeof(char*)* 100);
  for (x = 0; x < 100; x++)
    out[x] = (char*)malloc(sizeof(char)* 255);
#ifndef WIN32
  char* colorscheme;
  char* lineborder2;
  ///<Summary>
  ///Get color scheme (For linux only)
  ///</Summary>
  sprintf(queries[ind++], "SELECT option, text FROM OPTIONS WHERE option = 21");
  retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
  if (retval) {
#ifdef Console
    printf("Failed to get color scheme\n\r");
#endif
  }
  while (sqlite3_step(stmt) == SQLITE_ROW)
  if (strcmp((const char*)sqlite3_column_text(stmt, 0), "21") == 0) {
    colorscheme = (char*)calloc(50, sizeof(char));
    if (strcmp((const char*)sqlite3_column_text(stmt, 1), "black") == 0)
      sprintf(colorscheme, "%c[%d;%d;%dm", 0x1B, 1, 6, 66);
    else if (strcmp((const char*)sqlite3_column_text(stmt, 1), "blink") == 0)
      sprintf(colorscheme, "%c[%d;%d;%dm", 0x1B, 1, 50, 5);
    else if (strcmp((const char*)sqlite3_column_text(stmt, 1), "green") == 0)
      sprintf(colorscheme, "%c[%d;%d;%dm", 0x1B, 1, 68, 32);
    else if (strcmp((const char*)sqlite3_column_text(stmt, 1), "pink") == 0)
      sprintf(colorscheme, "%c[%d;%d;%dm", 0x1B, 1, 35, 2);
    else if (strcmp((const char*)sqlite3_column_text(stmt, 1), "red") == 0)
      sprintf(colorscheme, "%c[%d;%d;%dm", 0x1B, 1, 37, 41);
    else sprintf(colorscheme, "%c[%d;%d;%dm", 0x1B, 1, 37, 41);
    break;
  }
#endif
  if (parcount > 0)
#ifdef _MSC_VER
    sprintf_s(queries[ind++], q_size, "SELECT COALESCE(MAX(id),0) FROM TODO WHERE list = %d", list);
#else
    sprintf(queries[ind++], "SELECT COALESCE(MAX(id),0) FROM TODO WHERE list = %d", list);
#endif
  else queries[ind++] = "SELECT COALESCE(MAX(id),0) from TODO";
  retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
  if (retval) {
#ifdef Console
    printf("Reading DB data Failed, running re-init\n\r");
#endif
    if (todo_initdb() == 0) {
#ifdef Console
      printf("Done\n\r");
#endif
    } else {
#ifdef Console
      shitHappended();
#endif
      return NULL;
    }
    return todo_read_meta(list, parcount);
  }
  while (sqlite3_step(stmt) == SQLITE_ROW)
    maxl1 = strlen((const char*)sqlite3_column_text(stmt, 0));
  if (parcount > 0)
#ifdef _MSC_VER
    sprintf_s(queries[ind++], q_size, "SELECT COALESCE(MAX(LENGTH(text)),0) FROM TODO WHERE list = %d", list);
#else
    sprintf(queries[ind++], "SELECT COALESCE(MAX(LENGTH(text)),0) FROM TODO WHERE list = %d", list);
#endif
  else queries[ind++] = "SELECT COALESCE(MAX(LENGTH(text)),0) from TODO";
  retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
  if (retval) {
#ifdef Console
    printf("Reading DB data Failed, running re-init\n\r");
#endif
    if (todo_initdb() == 0) {
#ifdef Console
      printf("Done\n\r");
#endif
    } else {
#ifdef Console
      shitHappended();
#endif
      return NULL;
    }
    return todo_read_meta(list, parcount);
  }
  while (sqlite3_step(stmt) == SQLITE_ROW)
    maxl2 = atoi((const char*)sqlite3_column_text(stmt, 0));
  if (parcount > 0)
#ifdef _MSC_VER
    sprintf_s(queries[ind++], q_size, "SELECT id, text, LENGTH(text) FROM TODO WHERE list = %d", list);
#else
    sprintf(queries[ind++], "SELECT id, text, LENGTH(text) FROM TODO WHERE list = %d", list);
#endif
  else queries[ind++] = "SELECT id, text, LENGTH(text) from TODO";
  retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
  if (retval) {
#ifdef Console
    printf("Reading DB data Failed, running re-init\n\r");
#endif
    if (todo_initdb() == 0) {
#ifdef Console
      printf("Done\n\r");
#endif
    } else {
#ifdef Console
      shitHappended();
#endif
      return NULL;
    }
    return todo_read_meta(list, parcount);
  }
  lineborder1 = (char*)calloc(255, sizeof(char));
#ifndef WIN32
  lineborder2 = (char*)calloc(255, sizeof(char));
#endif
  spaces1 = (char*)calloc(200, sizeof(char));
  spaces2 = (char*)calloc(200, sizeof(char));
  for (i = 0; i < ((maxl2 + maxl1) + 5); i++)
  if (i == 2 + maxl1) {
#ifdef WIN32
#ifdef _MSC_VER
    strcat_s(lineborder1, 200, "+");
#else
    strcat(lineborder1, "+");
#endif
#else
    strcat(lineborder1, "╤");
    strcat(lineborder2, "╧");
#endif
  } else {
#ifdef WIN32
#ifdef _MSC_VER
    strcat_s(lineborder1, 200, "-");
#else
    strcat(lineborder1, "-");
#endif
#else
    strcat(lineborder1, "═");
    strcat(lineborder2, "═");
#endif
  }
#ifdef WIN32
#ifdef _MSC_VER
  sprintf_s(out[0], 255, "%s", lineborder1);
#else
  sprintf(out[0], "%s", lineborder1);
#endif
  x = 2;
#else
  sprintf(out[1], "%s", colorscheme);
  sprintf(out[0], "%s", lineborder1);
  x = 4;
#endif
  while (sqlite3_step(stmt) == SQLITE_ROW) {
    maxi1 = maxl1 - strlen((const char*)sqlite3_column_text(stmt, 0));
    maxi2 = maxl2 - atoi((const char*)sqlite3_column_text(stmt, 2));
#ifdef _MSC_VER
    strcpy_s(spaces1, 200, "");
    strcpy_s(spaces2, 200, "");
#else
    strcpy(spaces1, "");
    strcpy(spaces2, "");
#endif
    for (i = 0; i < maxi1; i++)
#ifdef _MSC_VER
      strcat_s(spaces1, 200, " ");
#else
      strcat(spaces1, " ");
#endif
    for (i = 0; i < maxi2; i++)
#ifdef _MSC_VER
      strcat_s(spaces2, 200, " ");
#else
      strcat(spaces2, " ");
#endif
#ifdef WIN32
#ifdef _MSC_VER
    sprintf_s(out[x], 255, "%s %s",
      sqlite3_column_text(stmt, 0)
      , spaces1);
    sprintf_s(out[x + 1], 255, "%s %s",
      sqlite3_column_text(stmt, 1)
      , spaces2);
#else
    sprintf(out[x], "%s %s",
      sqlite3_column_text(stmt, 0)
      , spaces1);
    sprintf(out[x + 1], "%s %s",
      sqlite3_column_text(stmt, 1)
      , spaces2);
#endif
#else
    sprintf(out[x], " %s %s",
      sqlite3_column_text(stmt, 0)
      , spaces1);
    sprintf(out[x + 1], " %s %s"
      , sqlite3_column_text(stmt, 1)
      , spaces2);
#endif
    x += 2;
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
  return out;
}
char** todo_read(int list, int parcount) {
  if (prelude() == -1) return NULL;
  return todo_read_meta(list, parcount);
}
char** todo_read_custom(int list, int parcount, char* db) {
  if (prelude_custom(db) == -1) return NULL;
  return todo_read_meta(list, parcount);
}

int todo_write_meta(char** argv, int argc, int list) {
  char* ending = (char*)calloc(200, sizeof(char));
  char first = 0;
  int last = 0;
  int argi;
  char* text;
  int useending = 0;
  int limit = 200;
  ///<Summary>
  ///Getting options from local database
  ///<Summary>
#ifdef _MSC_VER
  sprintf_s(queries[ind++], q_size, "SELECT option, text FROM OPTIONS WHERE option = 12 or option = 13");
#else
  sprintf(queries[ind++], "SELECT option, text FROM OPTIONS WHERE option = 12 or option = 13");
#endif
  retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
  if (retval) {
    printf("Reading DB data Failed, running re-init\n\r");
    if (todo_initdb() == 0) {
#ifdef Console
      printf("Done\n\r");
#endif
    } else {
#ifdef Console
      shitHappended();
#endif
      return -1;
    }
    return todo_write_meta(argv, argc, list);
  }
  while (sqlite3_step(stmt) == SQLITE_ROW)
  if (strcmp((const char*)sqlite3_column_text(stmt, 0), "13") == 0) {
#ifdef _MSC_VER
    sprintf_s(ending, 200, "%s", sqlite3_column_text(stmt, 1));
#else
    sprintf(ending, "%s", sqlite3_column_text(stmt, 1));
#endif
  } else if (strcmp((const char*)sqlite3_column_text(stmt, 0), "12") == 0) {
    useending = atoi((const char*)sqlite3_column_text(stmt, 1));
    }
  ///<Summary>
  ///Writing to local database
  ///<Summary>
#ifdef _MSC_VER
  sprintf_s(queries[ind++], q_size, "SELECT COALESCE(MAX(id),0) FROM TODO");
#else
  sprintf(queries[ind++], "SELECT COALESCE(MAX(id),0) FROM TODO");
#endif
  retval = sqlite3_prepare_v2(handle, queries[ind - 1], -1, &stmt, 0);
  if (retval) {
#ifdef Console
    printf("Reading DB data Failed, running re-init\n\r");
#endif
    if (todo_initdb() == 0) {
#ifdef Console
      printf("Done\n\r");
#endif
    } else {
#ifdef Console
      shitHappended();
#endif
      return -1;
    }
    return todo_write_meta(argv, argc, list);
  }
  while (sqlite3_step(stmt) == SQLITE_ROW)
    last = atoi((const char*)sqlite3_column_text(stmt, 0));
  text = (char*)calloc(200, sizeof(char));
  if (useending == 1)
    limit = 200 - strlen(ending);
  for (argi = 1; argi < argc; argi++) {
        if (strlen(text) + strlen(argv[argi]) + sizeof(char) >= (unsigned int)limit)
            break;
        else if ((strcmp(argv[argi], "--motivate") == 0))
            useending = 1;
        else if ((strcmp(argv[argi], "--first") == 0) || (strcmp(argv[argi], "-1") == 0))
            first = 1;
        else {
#ifdef _MSC_VER
            strcat_s(text, 200, argv[argi]);
            strcat_s(text, 200, " ");
#else
            strcat(text, argv[argi]);
            strcat(text, " ");
#endif
        }
    }
  if (useending == 1)
#ifdef _MSC_VER
    strcat_s(text, 200, ending);
#else
    strcat(text, ending);
#endif
  if (first == 1) {
    sql("UPDATE TODO SET id = id + 1000000000");
    sql("UPDATE TODO SET id = id - (1000000000 - 1)");
#ifdef _MSC_VER
    sprintf_s(queries[ind++], q_size, "INSERT INTO TODO VALUES(0,'%s',%d)", text, list);
#else
    sprintf(queries[ind++], "INSERT INTO TODO VALUES(0,'%s',%d)", text, list);
#endif
  } else
#ifdef _MSC_VER
    sprintf_s(queries[ind++], q_size, "INSERT INTO TODO VALUES(%d,'%s', %d)", last + 1, text, list);
#else
    sprintf(queries[ind++], "INSERT INTO TODO VALUES(%d,'%s', %d)", last + 1, text, list);
#endif
  retval = sqlite3_exec(handle, queries[ind - 1], 0, 0, 0);
  if (retval) {
#ifdef Console
    printf("Task were not added! (shit happens)\n\r");
#endif
    return -1;
  }
  free(text);
  free(ending);
  timeUpdate(time(0));
  return 0;
}
int todo_write(char** argv, int argc, int list) {
  if (prelude() == -1) return -1;
  return todo_write_meta(argv, argc, list);
}
int todo_write_custom(char** argv, int argc, int list, char* db) {
  if (prelude_custom(db) == -1) return -1;
  return todo_write_meta(argv, argc, list);
}
