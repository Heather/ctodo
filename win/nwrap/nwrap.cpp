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
#include "stdafx.h"
#include "nwrap.h"
//________________________________________________________________________________
namespace nwrap {
//________________________________________________________________________________
    System::String^ todo::fromchar(char* ch) {
        return gcnew String(ch);
        }
    char* todo::tochar(System::String^ str) {
        System::IntPtr a = System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(str);
        char* s = ((char*)(void*)a);
        return s;
        }
    #pragma warning(disable : 4996)
    char* todo::WcsToMbs(LPOLESTR ptName) {
        long sizew = wcstombs(NULL, ptName, 0);
        char* szName = new char[sizew + 1];
        setlocale(LC_CTYPE, "Russian_Russia.1251");
        wcstombs(szName, ptName, sizew + 1);
        return szName;
        }
    byte* todo::GetNative(array<System::Byte, 1> ^ byteArray) {
        try {
            pin_ptr<unsigned char> array_pin = &byteArray[0];
            return (byte*) array_pin;
            }
        catch(...) {
            return 0;
            }
        }
    wchar_t* todo::wconv(System::String ^ str) {
        pin_ptr<const wchar_t> wchstr = PtrToStringChars(str);
        return const_cast<wchar_t*>(wchstr);
        }
//________________________________________________________________________________
    todo::~todo() {
        free(argv);
        System::Runtime::InteropServices::Marshal::FreeHGlobal(p);
        todo_close();
        }
    void todo::n_initdb() {
        todo_initdb();
        }
    System::String^ todo::n_version() {
        return fromchar(todo_version());
        }
    System::String^  todo::n_help() {
        return fromchar(todo_help());
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
    System::Collections::Generic::List<System::String ^> ^ todo::n_read(int index, int params) {
        char** out = todo_read(index, params);
        int maxl;
        memcpy(&maxl, out[1], sizeof(int));
        System::Collections::Generic::List<System::String ^> ^ todolist = 
            gcnew System::Collections::Generic::List<System::String ^>();
        for(int x = 2; x < maxl; x++) {
            todolist->Add( fromchar(out[x]) );
            }
        return todolist;
        }
    int  todo::n_write(cli::array<System::String ^> ^ input, int argc) {
        int result;
        argv = (char**)malloc(sizeof(char*) * (3));
        /*for(int x = 0; x < 2; x++) {
            argv[x] = (char*)malloc(sizeof(char) * 255);
            }*/
        argv[0] = "";
        argv[1] = "test";
        /*for(int x = 0; x < argc; x++) {
            char* go = tochar(input[x]);
            argv[x + 1] = go;
            }*/
        try {
            result = todo_write(argv, 2); //argc + 1);
            return result;
            }
        catch(char* error) {
            errorMessage = fromchar(error);
            return -1;
            }
        }
    }
//________________________________________________________________________________