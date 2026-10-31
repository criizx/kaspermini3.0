# KasperMini v2.2

A cross-platform, multi-threaded malware scanner that performs MD5-based signature detection against a configurable hash database.

## Features

- **Cross-Platform Support**: Windows, Linux, and macOS compatibility
- **Multi-Threading**: Utilizes all available CPU cores for parallel file processing
- **Priority-Based Scanning**: Smart file prioritization system based on file extensions
- **Comprehensive Logging**: Detailed CSV-based logging with timestamps
- **Memory Efficient**: Optimized for large directory scans with minimal memory footprint
- **Static Linking**: Self-contained executables with minimal dependencies

## Architecture

The project is structured as a modular C++ application with the following components:

- `src/ScannerLib`: signature database, streaming MD5 calculation, parallel scanning and logging.
- `src/ScannerApp`: command-line parsing and scan reports.
- `tests`: GoogleTest tests for the library.

## Dependencies

- **CMake**: Version 3.16 or higher
- **OpenSSL**: For MD5 hash calculation
- **GoogleTest**: For testing

### Specific Requirements

#### Windows (MSVC)
OpenSSL installation path: `C:/OpenSSL-Win64/`

## Build

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

GoogleTest must be discoverable by CMake (use `CMAKE_PREFIX_PATH` for a custom installation).
To build only the application, configure with `-DBUILD_TESTING=OFF`.

### Signature database format

Each line uses `MD5;Verdict`, for example:

```text
d41d8cd98f00b204e9800998ecf8427e;Example.EmptyFile
```

Hashes are case-insensitive and must contain exactly 32 hexadecimal characters.
Blank lines and comments starting with `#` are ignored. Invalid entries and empty
verdicts are skipped. A failed or empty database reload preserves the previous database.

## Output

```
KasperMini v2.2 (Windows)
Files processed:    542
Suspicious files:    1
Execution time:     2.34 seconds
```

### Log File Format

```csv
Timestamp,FilePath,Hash,Verdict
2025-09-08 14:30:15,C:\temp\malware.exe,d41d8cd98f00b204e9800998ecf8427e,Trojan.Generic
```

## GPU Processing (Planned)

Originally planned GPU-based MD5 calculation with the following logic:

**If GPU is available:**
- **Files < 10MB**: CPU processing
- **Files 10-100MB**: Single GPU processing  
- **Files > 100MB**: Batch GPU processing
