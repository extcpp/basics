#pragma once

#ifdef linux
    #include <dlfcn.h>
#ifdef _WIN32
    #include <windows.h>
    #include <strsafe.h>
    #include "windows_strings.hpp"
    #include <cstdlib>
    #include <string>
#endif

#include <memory>

// filenames must be a utf-8 strings!

namespace obi { namespace util {
// types
    #ifdef linux
        typedef void*   DLHandle;
        typedef void*   DLAddress;
        typedef int     DLRv;
    #ifdef _WIN32
        typedef HMODULE DLHandle;
        typedef FARPROC DLAddress;
        typedef BOOL    DLRv;
    #endif
    typedef char*       UTF8Str;
    typedef std::string DLError;

// open
// returns NULL on fail
    DLHandle dl_open(const UTF8Str filename, int flag=TRLD_LAZY){
    #ifdef linux
        return ::dlopen(filename, flag);
    #ifdef _WIN32
        #ifdef UNICODE
            SPLPWSTR tmp = string_to_win(std::string(*filename));
            DLHandle rv  = ::LoadLibrary(tmp.get());
            return rv;
        #else
            //Requires LPSTR!! - todo use multibyte to wide
            return ::LoadLibraryA(filename);
        #endif
    #endif
    }

    //utf-8 encoded std::string
    DLHandle dl_open(const std::string& filename, int flag=TRLD_LAZY){
        dl_open(filename.c_str(), flag);
    }

// error
// returns UFT8Str - todo
// NOTE - you need to free the returned buffer on windows!!!
    DLError dl_error(void){
    #ifdef linux
        //returns a static buffer - do not free!!!!
        char* buffer = ::dlerror();
        if(buffer){
            return std::string(*buffer);
        } else {
            return std::string("");
        }
    #ifdef _WIN32
        LPVOID lpMsgBuf;
        DWORD  dw_error_num = ::GetLastError();

        ::FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            dw_error_num,
            ::MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            (LPTSTR) &lpMsgBuf,
            0, NULL
        );
        #ifdef UNICODE
            std::string rv = string_from_win(lpMsgBuf)
        #else
            std::string rv(*lpMsgBuf);
        #endif
        LocalFree(lpMsgBuf);
        return rv;
    #endif
    }


// get symbol
// returns NULL on fail
    DLAddress dl_sym(DLHandle handle, const UTF8Str symbol){
    #ifdef linux
        return ::dlsym(handle, symbol);
    #ifdef _WIN32
        return ::GetProcAddress(handle, symbol);
    #endif
    }


// close
// returns 0 on fail
    DLRv dl_close(DLHandle handle){
    #ifdef linux
        return ::dlclose(handle);
    #ifdef _WIN32
        return ::FreeLibrary(handle);
    #endif
    }

}}
