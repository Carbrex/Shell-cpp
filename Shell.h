#ifndef SHELL_H
#define SHELL_H

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

struct Command;
class Shell
{
public:
    void executeCommand(const Command &parsedCommand);
    Command parseCommand(const std::string &userInput);

private:
    void executeCd(const Command &parsedCommand);
    void executeMv(const Command &parsedCommand);
    void executeRm(const Command &parsedCommand);
    void executeLs(const Command &parsedCommand);
    void executeCp(const Command &parsedCommand);
    bool isWildcard(const char *path);
    // void listFilesRecursively(const fs::path &directory, bool showHidden);

    // Helper functions for wildcard support
    std::vector<std::string> expandWildcards(const std::string &pattern);
    // std::vector<std::string> expandWildcards(const std::string& pattern);
};

#endif // SHELL_H
