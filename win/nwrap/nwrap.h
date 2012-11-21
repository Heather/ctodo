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
