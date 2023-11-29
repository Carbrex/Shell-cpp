#include "cd.h"
#include <cerrno>

void CD::displayCurrentDirectory()
{
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != nullptr)
    {
        std::cout << "Current directory: " << cwd << std::endl;
    }
    else
    {
        perror("getcwd");
    }
}

void CD::changeDirectory(const std::string &path)
{
    if (chdir(path.c_str()) != 0)
    {
        perror("chdir");
    }
}

void CD::displayHelp()
{
    std::cout << "Usage: cd [options] [directory]\n"
              << "Change the current directory.\n\n"
              << "Options:\n"
              << "  -l      Display the current directory\n"
              << "  -h      Display this help message\n";
}
int CD::executeCd(int argc, char *argv[])
{
    int opt;
    bool displayCurrentDir = false;
    bool help = false;

    while ((opt = getopt(argc, argv, "lh")) != -1)
    {
        switch (opt)
        {
        case 'l':
            displayCurrentDir = true;
            break;
        case 'h':
            help = true;
            break;
        case '?':
            std::cerr << "Unknown option: -" << char(optopt) << std::endl;
            return 1;
        default:
            return 1;
        }
    }

    if (help)
    {
        displayHelp();
        return 0;
    }

    if (displayCurrentDir)
    {
        displayCurrentDirectory();
    }
    else if (optind < argc)
    {
        changeDirectory(argv[optind]);
    }
    else
    {
        std::cerr << "Error: Missing directory argument.\n";
        return 1;
    }

    return 0;
}
int main(int argc, char *argv[])
{
    CD cd;
    return cd.executeCd(argc,argv);
}
