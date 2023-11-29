#ifndef CD_H
#define CD_H

#include <iostream>
#include <unistd.h>
#include <cstring>

class CD
{
public:
    void displayCurrentDirectory();
    void changeDirectory(const std::string &path);
    void displayHelp();
    int executeCd(int argc, char *argv[]);
};

#endif // CD_H