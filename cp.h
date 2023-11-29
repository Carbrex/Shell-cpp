#ifndef CP_H
#define CP_H

#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdlib>
#include <dirent.h>
#include <cstring>
#include <fstream>

class CP
{
public:
    bool fileExists(const std::string &path);
    bool isDirectory(const std::string &path);
    void copyFile(const std::string &source, const std::string &destination, bool interactive, bool verbose);
    void copyDirectoryRecursive(const std::string &source, const std::string &destination, bool interactive, bool verbose);
    void displayHelp();
    int executeCp(int argc, char *argv[]);
};

#endif // CP_H
