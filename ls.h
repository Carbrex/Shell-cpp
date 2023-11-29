#ifndef LS_H
#define LS_H

#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <algorithm>
#include <cstring>
#include <filesystem>

class LS
{
public:
    bool fileExists(const std::string &path);
    void listFiles(const char *path, bool showHidden, bool reverseOrder, bool longFormat);
    void listFilesRecursive(const char *path, bool showHidden, bool reverseOrder, bool longFormat);
    void displayHelp();
    int executeLS(int argc, char *argv[]);
    bool isWildcard(const char *path);
};

#endif // LS_H
