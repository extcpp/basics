#pragma once
#ifdef _WIN32

#include <windows.h>
#if __cplusplus > 199711L
    #include <memory>
    typedef std::shared_ptr<LPWSTR> obi::util::SPLPWSTR;
#else
    #include <tr1/memory>
    typedef std::tr1::shared_ptr<LPWSTR> obi::util::SPLPWSTR;
#endif

// I define char* utf-8 encoded to be the default
// the order out in is not lucky but instring_size is the only optional argument

namespace obi { namespace util {

//              non UNICODE             UNICODE
//  LPCTSTR:    LPCSTR (const char*)    LPCWSTR (const wchar)
//  LPTSTR:     LPSTR (char*)           LPWSTR (wchar)

    int string_from_win_get_size(LPCWSTR in_string, const int& in_string_size=-1){
        return ::WideCharToMultiByte(
            CP_UTF8,                // UNIT    CodePage
            WC_ERR_INVALID_CHARS,   // DOWRD   dwFlags
            in_string,              // LPCWSTR lpWideCharStr
            in_string_size,         // int     cchWideChar
            NULL,                   // LPSTR   lpMultiByteStr -> char* (OUTPUT)
            0,                      // int     cbMultiByte    -  set to 0 to get required size
            NULL,                   // LPCSTR  lpDefaultChar
            NULL                    // LPBOOL  lpUsedDefaultChar
        );
    };

    int string_from_win(char* out_string, int out_string_size, LPCWSTR in_string, const int& in_string_size=-1){
        return ::WideCharToMultiByte(
            CP_UTF8,                // UNIT    CodePage
            WC_ERR_INVALID_CHARS,   // DOWRD   dwFlags
            in_string,              // LPCWSTR lpWideCharStr
            -1,                     // int     cchWideChar
            out_string,             // LPSTR   lpMultiByteStr -> char*
            out_string_size         // int     cbMultiByte
            NULL,                   // LPCSTR  lpDefaultChar
            NULL                    // LPBOOL  lpUsedDefaultChar
        );
    };

    std::string string_from_win(LPCWSTR in_string, const int& in_string_size=-1){
        int size = string_from_win_get_size(in_string, instring_size);
        std::alloc buffer(size * sizeof(char));
        string_from_win(buffer, size, in_string, instring_size);
        std::string rv = std::string(*buffer);
        std::free buffer;
        return rv;
    }

    int string_to_win_get_size(char* in_string, const int& in_string_size=-1){
        return ::MultiByteToWideChar(
            CP_UTF8,                // UINT     CodePage
            MB_PRECOMPOSED,         // DWORD    dwFlags
            in_string,              // LPCSTR   lpMultiByteStr
            in_string_size,         // int      cbMultiByte
            NULL,                   // LPWSTR   lpWideCharStr
            0                       // int      cchWideChar - 0 to ask for required size
        );
    };

    int string_to_win(LPWSTR out_string, int out_string_size, char* in_string, const int& in_string_size=-1){
        return ::MultiByteToWideChar(
            CP_UTF8,                // UINT     CodePage
            MB_PRECOMPOSED,         // DWORD    dwFlags
            in_string,              // LPCSTR   lpMultiByteStr
            in_string_size,         // int      cbMultiByte
            out_string,             // LPWSTR   lpWideCharStr
            out_string_size         // int      cchWideChar
        );
    };

    SPLPWSTR string_to_win(std::string in_string){
        int size = string_to_win_get_size(in_string);
        std::alloc buffer(size * sizeof(LPWSTR));
        string_to_win(buffer, size, in_string);
        #if __cplusplus > 199711L
            SPLPWSTR rv =      std::make_shared<LPWSTR>(*buffer);
        #else
            SPLPWSTR rv = std::tr1::make_shared<LPWSTR>(*buffer);
        #endif
        std::free buffer;
        return rv;
    }
}}
#endif
