cmake_minimum_required(VERSION 3.16)

set(GTEST_ROOT "C:/msys64/mingw64" CACHE PATH "Path to GoogleTest installation")
find_path(GTEST_INCLUDE_DIR gtest/gtest.h
        HINTS ${GTEST_ROOT}/include
)
find_library(GTEST_LIBRARY
        NAMES gtest
        HINTS ${GTEST_ROOT}/lib
)
find_library(GTEST_MAIN_LIBRARY
        NAMES gtest_main
        HINTS ${GTEST_ROOT}/lib
)

if(NOT GTEST_INCLUDE_DIR OR NOT GTEST_LIBRARY OR NOT GTEST_MAIN_LIBRARY)
    message(FATAL_ERROR "GoogleTest not found. Ensure it is installed in C:/msys64/mingw64 or specify GTEST_ROOT.")
endif()

include_directories(${GTEST_INCLUDE_DIR})

add_executable(ScannerTests
        HashDBTest.cpp
        UtilsTest.cpp
        ScannerTest.cpp
)

target_link_libraries(ScannerTests
        ScannerLib
        ${GTEST_LIBRARY}
        ${GTEST_MAIN_LIBRARY}
)

target_include_directories(ScannerTests PRIVATE
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/src/ScannerLib/include
)

if(WIN32 AND NOT MSVC)
    add_custom_command(TARGET ScannerTests POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:ScannerLib>
            $<TARGET_FILE_DIR:ScannerTests>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "C:/msys64/mingw64/bin/libgcc_s_seh-1.dll"
            $<TARGET_FILE_DIR:ScannerTests>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "C:/msys64/mingw64/bin/libstdc++-6.dll"
            $<TARGET_FILE_DIR:ScannerTests>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "C:/msys64/mingw64/bin/libwinpthread-1.dll"
            $<TARGET_FILE_DIR:ScannerTests>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "C:/msys64/mingw64/bin/libcrypto-3-x64.dll"
            $<TARGET_FILE_DIR:ScannerTests>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "C:/msys64/mingw64/bin/libssl-3-x64.dll"
            $<TARGET_FILE_DIR:ScannerTests>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "C:/msys64/mingw64/bin/libgtest.dll"
            $<TARGET_FILE_DIR:ScannerTests>
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "C:/msys64/mingw64/bin/libgtest_main.dll"
            $<TARGET_FILE_DIR:ScannerTests>
            COMMENT "Copying MinGW, OpenSSL, GoogleTest, and ScannerLib DLLs to test directory"
    )
endif()

set_target_properties(ScannerTests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

enable_testing()
add_test(NAME ScannerTests COMMAND ScannerTests)