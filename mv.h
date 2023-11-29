#ifndef MV_H
#define MV_H

#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdlib>
#include <dirent.h>
#include <cstring>
#include <filesystem>

namespace fs = std::filesystem;

class MV {
public:
    bool fileExists(const std::string &path);
    void moveFile(const std::string &source, const std::string &destination, bool verbose);
    void displayHelp();
    int executeMV(int argc, char *argv[]);
};

#endif // MV_H
