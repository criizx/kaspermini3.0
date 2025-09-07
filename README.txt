KasperMini v2.2

A cross-platform, multi-threaded malware scanner that performs MD5-based signature detection against a configurable hash database.

Features

- Cross-Platform Support: Windows, Linux, and macOS compatibility
- Multi-Threading: Utilizes all available CPU cores for parallel file processing
- Priority-Based Scanning: Smart file prioritization system based on file extensions
- Comprehensive Logging: Detailed CSV-based logging with timestamps
- Memory Efficient: Optimized for large directory scans with minimal memory footprint
- Static Linking: Self-contained executables with minimal dependencies

Architecture

The project is structured as a modular C++ application with the following components:

Dependencies

- CMake: Version 3.16 or higher
- OpenSSL: For MD5 hash calc
- GoogleTest: For testing

Specific Requirements

Windows (MSVC)
OpenSSL installation path
C:/OpenSSL-Win64/

Output

KasperMini v2.2 (Windows)
Files processed:    542
Suspicious files:    1
Execution time:     2.34 seconds

Log file:

Timestamp,FilePath,Hash,Verdict
2025-09-08 14:30:15,C:\temp\malware.exe,d41d8cd98f00b204e9800998ecf8427e,Trojan.Generic


Originally planned GPU-based MD5 calculation:

If there's GPU =>
- Files < 10MB: CPU processing
- Files 10-100MB: Single GPU processing
- Files > 100MB: Batch GPU processing
