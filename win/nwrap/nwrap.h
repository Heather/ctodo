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

#pragma once

extern "C" {
    #include "ctodo.h"
    }

using namespace System;

namespace nwrap {
	public ref class todo {
        public: 
            void n_initdb();
            void n_version();
            void n_help();
            int n_set(char** argv, int argc);
            int n_sync(char** argv);
            void n_edit(char** argv, int argc);
            void n_swap(char** argv);
            void n_mv(char** argv);
            void n_clean();
            void n_rm(char** argv);
            int n_read(int args);
            int n_write(char** argv, int argc);
	    };
    }
