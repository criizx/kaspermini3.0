#pragma once

#ifdef _WIN32
    #ifdef SCANNER_EXPORTS
        #define SCANNER_API __declspec(dllexport)
    #else
        #define SCANNER_API __declspec(dllimport)
    #endif
#else
    #define SCANNER_API
#endif