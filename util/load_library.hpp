// Copyright - 2015 - Jan Christoph Uhde <Jan@UhdeJC.com>

// Collection of functions that allows loading of libraries
// cross platform for linux and windows. Other platforms are
// not supported yet. All strings are expected to be be utf-8
// encoded.
// For more information please refer to:
// - man 3 dlopen or http://linux.die.net/man/3/dlopen
// - https://msdn.microsoft.com/en-us/library/windows/desktop/ms684175.aspx (LoadLibrary)
// - https://msdn.microsoft.com/en-us/library/windows/desktop/ms683212.aspx (GetProcAdress)
// - https://msdn.microsoft.com/en-us/library/windows/desktop/ms683152.aspx (FreeLibrary)

#pragma once

#ifdef __linux__
    #include <dlfcn.h>
#elif _WIN32
    #include <windows.h>
    #include <strsafe.h>
    #include "windows_strings.hpp"
#endif
#include <string>

namespace obi { namespace util {
// types
    #ifdef __linux__
        typedef void*   dl_handle;
        typedef void*   dl_address;
        typedef int     dl_rv;
    #elif _WIN32
        typedef HMODULE dl_handle;
        typedef FARPROC dl_address;
        typedef BOOL    dl_rv;
    #endif
    typedef char*       utf8_e_str;

    //! open library
    /*!
     * Function that opens dynamic library and loads it into the address space of current process
     * @param[in]   filename    path to library (null terminated string)
     * @param[in]   flag        binding strategy - defaults to RTLD_LAZY (ignored on windows)
     * @return                  dl_handle or NULL on fail
     */
    dl_handle dl_open(const utf8_e_str filename, int flag=RTLD_LAZY){
    #ifdef __linux__
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

    //! open library - overload for std::string
    dl_handle dl_open(const std::string& filename, int flag=RTLD_LAZY){
        dl_open(filename.c_str(), flag);
    }


    //! get error
    /*!
     * Functions that returns textual error
     * @return  textual description of the error in utf-8 encoded std::string
     */
    std::string dl_error(void){
    #ifdef __linux__
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
        ::LocalFree(lpMsgBuf);
        return rv;
    #endif
    }


    //! get address of symbol
    /*!
     *  Function that receives address of symbol in dynamic library
     *  @param[in]  handle      Handle to dynamic library
     *  @param[in]  symbol      function or variable name to look up
     *  @return     dl_address  pointer to requested symbol
     *                          or NULL if symbol is not found
     */
    dl_address dl_sym(dl_handle handle, const utf8_e_str symbol){
    #ifdef __linux__
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

    //! get address of symbol - overload for std::string
    dl_address dl_sym(dl_handle handle, const std::string& symbol){
        return dl_sym(handle, symbol.c_str());
    }


    //! close library
    /*!
     *  Function that decreases reference count on dynamic library and
     *  and closes the file if reference count drops to 0.
     *  @param[in]  handle  handle of lib to close
     *  @return             returns NULL on fail
     */
    dl_rv dl_close(dl_handle handle){
    #ifdef __linux__
        return ::dlclose(handle);
    #elif _WIN32
        return ::FreeLibrary(handle);
    #endif
    }
}}
