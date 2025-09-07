add_library(ScannerLib SHARED
        src/Scanner.cpp
        src/HashDB.cpp
        src/Utils.cpp
)

target_link_libraries(ScannerLib
        OpenSSL::Crypto
)

if(NOT WIN32)
    target_link_libraries(ScannerLib OpenSSL::SSL OpenSSL::Crypto)
else()
    target_link_libraries(ScannerLib OpenSSL::SSL)
endif()

if(WIN32 AND NOT MSVC)
    target_link_libraries(ScannerLib -static-libstdc++ -static-libgcc)
endif()

target_include_directories(ScannerLib
        PUBLIC include/
        PRIVATE src/
)

if(WIN32)
    target_compile_definitions(ScannerLib PRIVATE SCANNER_EXPORTS)

    set_target_properties(ScannerLib PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
            ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )
endif()