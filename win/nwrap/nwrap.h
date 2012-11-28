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
#pragma once
//________________________________________________________________________________
extern "C" {
    #include "ctodo.h"
    }
//________________________________________________________________________________
using namespace System;
//________________________________________________________________________________
namespace nwrap {
	public ref class todo {
    public: 
        ~todo();
        void n_initdb();
        String^ n_version();
        String^ n_help();
        int n_set(char** argv, int argc);
        int n_sync(char** argv);
        void n_edit(char** argv, int argc);
        void n_swap(char** argv);
        void n_mv(char** argv);
        void n_clean();
        void n_rm(char** argv);
        System::Collections::Generic::List<System::String ^> ^ n_read(int index, int params);
        int n_write(cli::array<System::String ^> ^ input, int argc);

        String^ errorMessage;
    private:
        char** argv;
        IntPtr p;
        char* tochar(System::String^ str);
        char* WcsToMbs(LPOLESTR ptName);
        byte* GetNative(array<System::Byte, 1> ^ byteArray);
        wchar_t* wconv(System::String ^ str);
        System::String^ fromchar(char* ch);
	    };
    }
//________________________________________________________________________________