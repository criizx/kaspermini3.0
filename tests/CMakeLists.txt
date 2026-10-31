if(WIN32)
    set(GTEST_ROOT "C:/msys64/mingw64" CACHE PATH "Path to GoogleTest installation")
endif()
find_package(GTest REQUIRED)

add_executable(ScannerTests
        HashDBTest.cpp
        UtilsTest.cpp
        ScannerTest.cpp
)

target_link_libraries(ScannerTests PRIVATE ScannerLib GTest::Main)

set_target_properties(ScannerTests PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

if(WIN32)
    add_custom_command(TARGET ScannerTests POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:ScannerLib>
            $<TARGET_FILE_DIR:ScannerTests>
    )

    # Static GoogleTest installations do not have runtime DLLs.
    # Honour the selected dependency roots instead of fixing the MSYS2 path.
    set(test_runtime_candidates
            "${GTEST_ROOT}/bin/libgtest.dll"
            "${GTEST_ROOT}/bin/libgtest_main.dll"
            "${OPENSSL_ROOT_DIR}/bin/libcrypto-3-x64.dll"
    )
    if(MINGW)
        get_filename_component(compiler_bin "${CMAKE_CXX_COMPILER}" DIRECTORY)
        list(APPEND test_runtime_candidates
                "${compiler_bin}/libgcc_s_seh-1.dll"
                "${compiler_bin}/libstdc++-6.dll"
                "${compiler_bin}/libwinpthread-1.dll"
                "${compiler_bin}/libcrypto-3-x64.dll"
                "${compiler_bin}/libssl-3-x64.dll"
        )
    endif()
    foreach(runtime IN LISTS test_runtime_candidates)
        if(EXISTS "${runtime}")
            add_custom_command(TARGET ScannerTests POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${runtime}" $<TARGET_FILE_DIR:ScannerTests>
            )
        endif()
    endforeach()
endif()

add_test(NAME ScannerTests COMMAND ScannerTests)
