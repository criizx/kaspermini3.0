#pragma once

#include <string>

struct CommandLineArgs {
    std::string basePath;
    std::string logPath;
    std::string scanPath;
};

bool ParseCommandLine(int argc, char* argv[], CommandLineArgs& args);
void PrintUsage();