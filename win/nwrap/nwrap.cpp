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

#include "stdafx.h"

#include "nwrap.h"

namespace nwrap {
    void  todo::n_initdb() {
        todo_initdb();
        }
    void  todo::n_version() {
        todo_version();
        }
    void  todo::n_help() {
        char* argv;
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
    int  todo::n_read(int args) {
        char** argv;
        return todo_read(argv, args);
        }
    int  todo::n_write(char** argv, int argc) {
        return todo_write(argv, argc);
        }
    }