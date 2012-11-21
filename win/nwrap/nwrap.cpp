// This is the main DLL file.

#include "stdafx.h"

#include "nwrap.h"

namespace nwrap {
    void  todo::n_initdb() {
        todo_initdb();
        }
    void  todo::n_version() {
        todo_version();
        }
    void  todo::n_help(char* argv) {
        todo_help(argv);
        }
    int  todo::n_set(char** argv, int argc) {
        return todo_set(argv, argc);
        }
    int  todo::n_sync(char** argv) {
        return todo_sync(argv);
        }
    void  todo::n_edit(char** argv, int argc) {
        todo_edit(argv, argc);
        }
    void  todo::n_swap(char** argv) {
        todo_swap(argv);
        }
    void  todo::n_mv(char** argv) {
        todo_mv(argv);
        }
    void  todo::n_clean() {
        todo_clean();
        }
    void  todo::n_rm(char** argv) {
        todo_rm(argv);
        }
    int  todo::n_read(char** argv, int argc) {
        return todo_read(argv, argc);
        }
    int  todo::n_write(char** argv, int argc) {
        return todo_write(argv, argc);
        }
    }