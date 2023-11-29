#ifndef RM_H
#define RM_H

#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <filesystem>
#include <vector>
#include <algorithm>

class RM
{
public:
    void removeFile(const std::filesystem::path &filePath, bool verbose);
    void removeDirectory(const std::filesystem::path &dirPath, bool verbose);
    std::vector<std::filesystem::path> expandWildcard(const std::filesystem::path &pattern);
    int executeRM(int argc, char *argv[]);
};

#endif // RM_H
