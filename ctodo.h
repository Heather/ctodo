//________________________________________________________________________________
#ifndef CTODO_H
#define CTODO_H
//________________________________________________________________________________
#ifdef WIN32
#include <Windows.h>
#endif
//________________________________________________________________________________
extern void todo_initdb();
extern void todo_initdb_custom(char* db);
extern char* todo_version();
extern char* todo_help();
extern int todo_set(char** argv, int argc);
extern int todo_set_custom(char** argv, int argc, char* db);
extern int todo_history();
extern int todo_sync(char** argv);
extern int todo_sync_custom(char** argv, char* db);
extern void todo_edit(char** argv, int argc);
extern void todo_edit_custom(char** argv, int argc, char* db);
extern void todo_swap(char** argv);
extern void todo_swap_custom(char** argv, char* db);
extern void todo_reindex();
extern void todo_mv(char** argv);
extern void todo_mv_custom(char** argv, char* db);
extern void todo_clean();
extern void todo_clean_custom();
extern void todo_rm(char** argv);
extern void todo_rm_custom(char** argv, char* db);
extern char** todo_read(int list, int parcount);
extern char** todo_read_custom(int list, int parcount, char* db);
extern int todo_write(char** argv, int argc, int list);
extern int todo_write_custom(char** argv, int argc, int list, char* db);
extern void todo_close();
//________________________________________________________________________________
#endif // CTODO_H
//________________________________________________________________________________
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
