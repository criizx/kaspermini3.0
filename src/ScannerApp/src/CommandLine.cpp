#include "../include/CommandLine.h"
#include <iostream>
#include <filesystem>
#include <cctype>  // Для trim

std::string Trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t");
    return str.substr(start, end - start + 1);
}

bool ParseCommandLine(int argc, char* argv[], CommandLineArgs& args) {
    if (argc == 2 && std::string(argv[1]) == "--help") {
        PrintUsage();
        exit(0);
    }

    if (argc < 7) {
        return false;
    }

    for (int i = 1; i < argc - 1; i += 2) {
        std::string arg = Trim(argv[i]);
        std::string value = Trim(argv[i + 1]);

        if (arg == "--help") {
            PrintUsage();
            exit(0);
        } else if (arg == "--base") {
            args.basePath = value;
        } else if (arg == "--log") {
            args.logPath = value;
        } else if (arg == "--path") {
            args.scanPath = value;
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            return false;
        }
    }

    if (args.basePath.empty() || args.logPath.empty() || args.scanPath.empty()) {
        return false;
    }

    args.basePath = Trim(args.basePath);
    args.logPath = Trim(args.logPath);
    args.scanPath = Trim(args.scanPath);

    if (!std::filesystem::exists(args.basePath)) {
        std::cerr << "Error: Base file does not exist: " << args.basePath << std::endl;
        return false;
    }

    if (!std::filesystem::exists(args.scanPath)) {
        std::cerr << "Error: Scan directory does not exist: " << args.scanPath << std::endl;
        return false;
    }

    return true;
}

void PrintUsage() {
    std::cout << "Usage: scanner.exe --base <base.csv> --log <report.log> --path <scan_directory>" << std::endl;
    std::cout << "Example: scanner.exe --base malware.csv --log scan_report.log --path c:\\temp" << std::endl;
    std::cout << "Use --help for this message." << std::endl;
}