#ifndef CD_H
#define CD_H

#include <iostream>
#include <unistd.h>
#include <cstring>

class CD {
public:
    void displayCurrentDirectory();
    void changeDirectory(const std::string &path);
    void displayHelp();
};

#endif // CD_H