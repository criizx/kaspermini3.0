cmake_minimum_required(VERSION 3.16)
project(MalwareScanner)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Определяем компилятор
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(MSVC TRUE)
else()
    set(MSVC FALSE)
endif()

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror)
    # Флаги для обхода проблем с MinGW
    add_compile_options(-Wno-deprecated-declarations)
    add_compile_definitions(_GLIBCXX_USE_STDIO_PURE=0)
endif()

if(WIN32)
    if(MSVC)
        # Настройки для MSVC
        set(OPENSSL_ROOT_DIR "C:/OpenSSL-Win64")
        set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")

        if(CMAKE_BUILD_TYPE STREQUAL "Debug")
            set(OPENSSL_LIB_DIR "${OPENSSL_ROOT_DIR}/lib/VC/x64/MDd")
        else()
            set(OPENSSL_LIB_DIR "${OPENSSL_ROOT_DIR}/lib/VC/x64/MD")
        endif()

        find_library(OPENSSL_SSL_LIBRARY libssl.lib PATHS ${OPENSSL_LIB_DIR} NO_DEFAULT_PATH)
        find_library(OPENSSL_CRYPTO_LIBRARY libcrypto.lib PATHS ${OPENSSL_LIB_DIR} NO_DEFAULT_PATH)

        if(NOT OPENSSL_SSL_LIBRARY OR NOT OPENSSL_CRYPTO_LIBRARY)
            message(FATAL_ERROR "OpenSSL libraries not found in ${OPENSSL_LIB_DIR}")
        endif()

        add_library(OpenSSL::SSL UNKNOWN IMPORTED)
        set_target_properties(OpenSSL::SSL PROPERTIES
                IMPORTED_LOCATION "${OPENSSL_SSL_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}"
        )

        add_library(OpenSSL::Crypto UNKNOWN IMPORTED)
        set_target_properties(OpenSSL::Crypto PROPERTIES
                IMPORTED_LOCATION "${OPENSSL_CRYPTO_LIBRARY}"
                INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}"
        )

        set(OPENSSL_FOUND TRUE)
    else()
        # Настройки для MinGW - используем MSYS2 OpenSSL
        set(OPENSSL_ROOT_DIR "C:/msys64/mingw64")
        find_package(OpenSSL REQUIRED)
        set(OPENSSL_FOUND TRUE)
    endif()

    message(STATUS "OpenSSL found: ${OPENSSL_FOUND}")
    message(STATUS "OpenSSL include: ${OPENSSL_INCLUDE_DIR}")
else()
    find_package(OpenSSL REQUIRED)
    set(OPENSSL_FOUND TRUE)
endif()

add_subdirectory(src/ScannerLib)
add_subdirectory(src/ScannerApp)