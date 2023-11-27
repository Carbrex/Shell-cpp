#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <filesystem>

namespace fs = std::filesystem;

void removeFile(const fs::path &filePath, bool verbose)
{
    try
    {
        fs::remove(filePath);
        if (verbose)
        {
            std::cout << "Removed file: " << filePath << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error removing file: " << e.what() << std::endl;
    }
}

void removeDirectory(const fs::path &dirPath, bool verbose)
{
    try
    {
        fs::remove_all(dirPath);
        if (verbose)
        {
            std::cout << "Removed directory and its contents: " << dirPath << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error removing directory: " << e.what() << std::endl;
    }
}

int main(int argc, char *argv[])
{
    int opt;
    bool recursive = false;
    bool interactive = false;
    bool verbose = false;

    while ((opt = getopt(argc, argv, "rivh")) != -1)
    {
        switch (opt)
        {
        case 'r':
            recursive = true;
            break;
        case 'i':
            interactive = true;
            break;
        case 'h':
            std::cout << "Help: This program mimics the behavior of the rm command with options.\n"
                      << "Options:\n"
                      << "  -r : Remove directories and their contents recursively.\n"
                      << "  -i : Interactive mode, ask for confirmation before removal.\n"
                      << "  -h : Display help.\n"
                      << "  -v : Verbose output.\n";
            std::exit(EXIT_SUCCESS);
        case 'v':
            verbose = true;
            break;
        default:
            std::cerr << "Usage: " << argv[0] << " [-r] [-i] [-h] [-v] <file/directory>\n";
            std::exit(EXIT_FAILURE);
        }
    }

    if (optind >= argc)
    {
        std::cerr << "Usage: " << argv[0] << " [-r] [-i] [-h] [-v] <file/directory>\n";
        std::cout << "Help: This program mimics the behavior of the rm command with options.\n"
                  << "Options:\n"
                  << "  -r : Remove directories and their contents recursively.\n"
                  << "  -i : Interactive mode, ask for confirmation before removal.\n"
                  << "  -h : Display help.\n"
                  << "  -v : Verbose output.\n";
        std::exit(EXIT_FAILURE);
    }

    fs::path pathToRemove = argv[optind];

    if (interactive)
    {
        std::cout << "Interactive mode is enabled. Be cautious!\n";
    }

    if (fs::exists(pathToRemove))
    {
        if (fs::is_directory(pathToRemove) && recursive)
        {
            if (interactive)
            {
                std::string response;
                std::cout << "Remove directory and its contents (y/n)? ";
                std::cin >> response;

                if (response == "y" || response == "Y")
                {
                    removeDirectory(pathToRemove, verbose);
                }
                else
                {
                    std::cout << "Operation canceled.\n";
                }
            }
            else
            {
                removeDirectory(pathToRemove, verbose);
            }
        }
        else
        {
            if (interactive)
            {
                std::string response;
                std::cout << "Remove file (y/n)? ";
                std::cin >> response;

                if (response == "y" || response == "Y")
                {
                    removeFile(pathToRemove, verbose);
                }
                else
                {
                    std::cout << "Operation canceled.\n";
                }
            }
            else
            {
                removeFile(pathToRemove, verbose);
            }
        }
    }
    else
    {
        std::cerr << "Error: Path does not exist: " << pathToRemove << std::endl;
        std::exit(EXIT_FAILURE);
    }

    return 0;
}
