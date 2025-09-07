#include <iostream>
#include <filesystem>
#include <iomanip>

#include "Scanner.h"
#include "CommandLine.h"

#ifdef _WIN32
    #define PLATFORM_NAME "Windows"
#elif defined(__linux__)
    #define PLATFORM_NAME "Linux"
#elif defined(__APPLE__)
    #define PLATFORM_NAME "macOS"
#else
    #define PLATFORM_NAME "Unknown"
#endif

int main(int argc, char* argv[]) {
    std::cout << "KasperMini v2.2 (" << PLATFORM_NAME << ")" << std::endl;
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
    auto [totalFiles, suspiciousFiles, errors, duration] = scanner.ScanDirectory(args.scanPath);

    std::cout << std::endl;
    std::cout << "========== SCAN REPORT ==========" << std::endl;
    std::cout << "Files processed:    " << totalFiles << std::endl;
    std::cout << "Suspicious files:    " << suspiciousFiles << std::endl;
    std::cout << "Errors encountered: " << errors << std::endl;
    std::cout << "Execution time:     " << std::fixed << std::setprecision(2)
              << duration << " seconds" << std::endl;
    std::cout << "=================================" << std::endl;

    if (suspiciousFiles > 0) {
        std::cout << "WARNING: Suspicious file/s detected! Check log file: " << args.logPath << std::endl;
        return 2;
    }

    std::cout << "Scan completed successfully." << std::endl;
    return 0;
}