# KasperMini v2.2 - High-Performance Malware Scanner

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

### Core Components

- **ScannerLib**: Shared library containing core scanning functionality
- **ScannerApp**: Command-line application interface
- **HashDB**: CSV-based malware signature database manager
- **Utils**: Cross-platform utility functions for MD5 calculation and string operations

### Key Classes

- `Scanner`: Main scanning engine with multi-threaded directory traversal
- `HashDB`: Hash database loader and lookup system
- `Utils`: Static utility methods for cryptographic operations
- `CommandLine`: Argument parsing and validation

## Build Requirements

### Dependencies

- **CMake**: Version 3.16 or higher
- **C++17** compatible compiler
- **OpenSSL**: For MD5 hash calculations
- **GoogleTest**: For unit testing (optional)

### Platform-Specific Requirements

#### Windows (MSVC)
```bash
# OpenSSL installation path
C:/OpenSSL-Win64/
```

## Usage

```bash
./scanner --base malware.csv --log scan_report.log --path /path/to/scan
```

### Command Line Arguments

- `--base <file.csv>`: Path to malware hash database (CSV format)
- `--log <file.log>`: Output log file for scan results
- `--path <directory>`: Directory to scan recursively
- `--help`: Display usage information

### Hash Database Format

The CSV hash database should follow this format:
```csv
# Comments start with #
md5_hash_32_chars;Verdict_Description
d41d8cd98f00b204e9800998ecf8427e;Trojan.Generic
a948904f2f0f479b8f8197694b30184b;Virus.Win32.Test
```

## Output

**Console:**
```
KasperMini v2.2 (Windows)
Files processed:    542
Suspicious files:    1
Execution time:     2.34 seconds
```

**Log file:**
```csv
Timestamp,FilePath,Hash,Verdict
2025-09-08 14:30:15,C:\temp\malware.exe,d41d8cd98f00b204e9800998ecf8427e,Trojan.Generic
```

## Future Plans

**GPU Acceleration**: Originally planned GPU-based MD5 calculation:
- Files < 10MB: CPU processing
- Files 10-100MB: Single GPU processing
- Files > 100MB: Batch GPU processing