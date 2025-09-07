add_executable(ScannerApp
        src/main.cpp
        src/CommandLine.cpp
)

target_include_directories(ScannerApp PRIVATE
        include/
        ${CMAKE_CURRENT_SOURCE_DIR}/../ScannerLib/include/
)

target_link_libraries(ScannerApp ScannerLib)

# Статическая линковка стандартных библиотек для MinGW
if(WIN32 AND NOT MSVC)
    target_link_libraries(ScannerApp -static-libstdc++ -static-libgcc)
endif()

if(WIN32)
    # Копируем DLL ScannerLib
    add_custom_command(TARGET ScannerApp POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:ScannerLib>
            $<TARGET_FILE_DIR:ScannerApp>
            COMMENT "Copying ScannerLib DLL to EXE directory"
    )

    # Копируем OpenSSL DLL
    if(OPENSSL_FOUND)
        if(MSVC)
            # Для MSVC копируем из стандартного расположения
            add_custom_command(TARGET ScannerApp POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${OPENSSL_ROOT_DIR}/bin/libssl-3-x64.dll"
                    $<TARGET_FILE_DIR:ScannerApp>
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "${OPENSSL_ROOT_DIR}/bin/libcrypto-3-x64.dll"
                    $<TARGET_FILE_DIR:ScannerApp>
                    COMMENT "Copying OpenSSL DLLs to EXE directory"
            )
        else()
            # Для MinGW копируем из MSYS2
            add_custom_command(TARGET ScannerApp POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "C:/msys64/mingw64/bin/libssl-3-x64.dll"
                    $<TARGET_FILE_DIR:ScannerApp>
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    "C:/msys64/mingw64/bin/libcrypto-3-x64.dll"
                    $<TARGET_FILE_DIR:ScannerApp>
                    COMMENT "Copying OpenSSL DLLs to EXE directory"
            )
        endif()
    endif()

    # Копируем MinGW runtime DLL если используем MinGW
    if(NOT MSVC)
        add_custom_command(TARGET ScannerApp POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "C:/Program Files/JetBrains/CLion 2023.*/bin/mingw/bin/libgcc_s_seh-1.dll"
                $<TARGET_FILE_DIR:ScannerApp>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "C:/Program Files/JetBrains/CLion 2023.*/bin/mingw/bin/libstdc++-6.dll"
                $<TARGET_FILE_DIR:ScannerApp>
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "C:/Program Files/JetBrains/CLion 2023.*/bin/mingw/bin/libwinpthread-1.dll"
                $<TARGET_FILE_DIR:ScannerApp>
                COMMENT "Copying MinGW runtime DLLs to EXE directory"
        )
    endif()

    set_target_properties(ScannerApp PROPERTIES OUTPUT_NAME "scanner")
    set_target_properties(ScannerApp PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endif()