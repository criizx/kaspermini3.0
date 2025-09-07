cmake_minimum_required(VERSION 3.16)
project(MalwareScanner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(MSVC TRUE)
else()
    set(MSVC FALSE)
endif()

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Werror -Wno-deprecated-declarations)
    add_compile_definitions(_GLIBCXX_USE_STDIO_PURE=0)
endif()

if(WIN32 AND MSVC)
    set(OPENSSL_ROOT_DIR "C:/OpenSSL-Win64" CACHE PATH "Path to OpenSSL installation")

    if(NOT EXISTS "${OPENSSL_ROOT_DIR}")
        message(WARNING "OpenSSL not found at ${OPENSSL_ROOT_DIR}")
        set(OPENSSL_ROOT_DIR "C:/OpenSSL" CACHE PATH "Path to OpenSSL installation" FORCE)
    endif()

    set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(OPENSSL_LIB_DIR "${OPENSSL_ROOT_DIR}/lib/VC/x64/MDd")
    else()
        set(OPENSSL_LIB_DIR "${OPENSSL_ROOT_DIR}/lib/VC/x64/MD")
    endif()

    if(NOT EXISTS "${OPENSSL_LIB_DIR}")
        if(EXISTS "${OPENSSL_ROOT_DIR}/lib")
            set(OPENSSL_LIB_DIR "${OPENSSL_ROOT_DIR}/lib")
        endif()
    endif()


    find_library(OPENSSL_SSL_LIBRARY
            NAMES libssl ssl ssleay32 libssl-3 libssl-3-x64
            PATHS ${OPENSSL_LIB_DIR}
            NO_DEFAULT_PATH
    )

    find_library(OPENSSL_CRYPTO_LIBRARY
            NAMES libcrypto crypto libeay32 libcrypto-3 libcrypto-3-x64
            PATHS ${OPENSSL_LIB_DIR}
            NO_DEFAULT_PATH
    )

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

elseif(WIN32)
    find_package(OpenSSL REQUIRED)
    set(OPENSSL_FOUND TRUE)
else()
    find_package(OpenSSL REQUIRED)
    set(OPENSSL_FOUND TRUE)
endif()

add_subdirectory(src/ScannerLib)
add_subdirectory(src/ScannerApp)
add_subdirectory(tests)