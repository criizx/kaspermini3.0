#include <iostream>
#include <filesystem>
#include <iomanip>
#include "Scanner.h"
#include "CommandLine.h"

#ifdef _WIN32
    #include <windows.h>
    #define PLATFORM_NAME "Windows"
#elif defined(__linux__)
    #define PLATFORM_NAME "Linux"
#elif defined(__APPLE__)
    #define PLATFORM_NAME "macOS"
#else
    #define PLATFORM_NAME "Unknown"
#endif

int main(int argc, char* argv[]) {
    std::cout << "Malware Scanner v1.0 (" << PLATFORM_NAME << ")" << std::endl;
    std::cout << "=======================================" << std::endl;

    CommandLineArgs args;
    if (!ParseCommandLine(argc, argv, args)) {
        PrintUsage();
        return 1;
    }

    Scanner scanner;
    if (!scanner.Initialize(args.basePath, args.logPath)) {
        std::cerr << "Failed to initialize scanner" << std::endl;
        return 1;
    }

    std::cout << "Configuration:" << std::endl;
    std::cout << "  Base file: " << args.basePath << std::endl;
    std::cout << "  Log file: " << args.logPath << std::endl;
    std::cout << "  Scan path: " << args.scanPath << std::endl;
    std::cout << "  Platform: " << PLATFORM_NAME << std::endl;
    std::cout << "  HashDB loaded: " << scanner.GetHashCount() << " entries" << std::endl;
    std::cout << std::endl;

    std::cout << "Starting scan of: " << args.scanPath << std::endl;
    auto result = scanner.ScanDirectory(args.scanPath);

    std::cout << std::endl;
    std::cout << "========== SCAN REPORT ==========" << std::endl;
    std::cout << "Files processed:    " << result.totalFiles << std::endl;
    std::cout << "Malicious files:    " << result.maliciousFiles << std::endl;
    std::cout << "Errors encountered: " << result.errors << std::endl;
    std::cout << "Execution time:     " << std::fixed << std::setprecision(2)
              << result.duration << " seconds" << std::endl;
    std::cout << "=================================" << std::endl;

    if (result.maliciousFiles > 0) {
        std::cout << "WARNING: Malicious files detected! Check log file: " << args.logPath << std::endl;
        return 2;
    }

    std::cout << "Scan completed successfully." << std::endl;
    return 0;
}