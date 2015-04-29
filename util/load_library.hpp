#pragma once

#ifdef linux
    #include <dlfcn.h>
#elif _WIN32
    #include <windows.h>
    #include <strsafe.h>
    #include "windows_strings.hpp"
#endif

#include <string>

// filenames must be a utf-8 strings!
// std::string or char* are allowed

namespace obi { namespace util {
// types
    #ifdef linux
        typedef void*   dl_handle;
        typedef void*   dl_address;
        typedef int     dl_rv;
    #elif _WIN32
        typedef HMODULE dl_handle;
        typedef FARPROC dl_address;
        typedef BOOL    dl_rv;
    #endif
    typedef char*       utf8_e_str;


// open
// returns NULL on fail
    dl_handle dl_open(const utf8_e_str filename, int flag=TRLD_LAZY){
    #ifdef linux
        return ::dlopen(filename, flag);
    #elif _WIN32
        #ifdef UNICODE
            std::wstring tmp = string_to_win(filename);
            return ::LoadLibrary(tmp.c_str());
        #else
            return ::LoadLibrary(filename);
        #endif
    #endif
    }

    //utf-8 encoded std::string
    dl_handle dl_open(const std::string& filename, int flag=TRLD_LAZY){
        dl_open(filename.c_str(), flag);
    }


// error
    std::string dl_error(void){
    #ifdef linux
        //returns a static buffer - do not free!!!!
        char* buffer = ::dlerror();
        if(buffer){
            return std::string(buffer);
        } else {
            return std::string("");
        }
    #elif _WIN32
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
            std::string rv(lpMsgBuf);
        #endif
        LocalFree(lpMsgBuf);
        return rv;
    #endif
    }


// get symbol
// returns NULL on fail
    dl_address dl_sym(dl_handle handle, const utf8_e_str symbol){
    #ifdef linux
        return ::dlsym(handle, symbol);
    #elif _WIN32
        #ifdef UNICODE
            std::wstring tmp = string_to_win(symbol);
            return ::GetProcAddress(handle, tmp.c_str());
        #else
            return ::GetProcAddress(handle, symbol);
        #endif
    #endif
    }

    dl_address dl_sym(dl_handle handle, const std::string& symbol){
        return dl_sym(handle, symbol.cstr());
    }

// close
// returns 0 on fail
    dl_rv dl_close(dl_handle handle){
    #ifdef linux
        return ::dlclose(handle);
    #elif _WIN32
        return ::FreeLibrary(handle);
    #endif
    }

}}
