#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <filesystem>
#include <vector>
#include <algorithm>

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

std::vector<fs::path> expandWildcard(const fs::path &pattern)
{
    std::vector<fs::path> result;
    for (const auto &entry : fs::directory_iterator(pattern))
    {
        result.push_back(entry.path());
    }
    return result;
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

    for (size_t i = optind; i < argc; i++)
    {
        fs::path pathToRemove = argv[i];
        if (pathToRemove.has_relative_path() && (pathToRemove.has_filename() && (pathToRemove.filename().string().find('*') != std::string::npos || pathToRemove.filename().string().find('?') != std::string::npos)))
        {
            std::vector<fs::path> expandedPaths = expandWildcard(pathToRemove);
            for (const auto &expandedPath : expandedPaths)
            {
                if (fs::exists(expandedPath))
                {
                    if (fs::is_directory(expandedPath))
                    {
                        if (!recursive)
                        {
                            std::cout << "rm: cannot remove " << expandedPath << ": Is a directory\n";
                            continue;
                        }
                    }

                    if (interactive)
                    {
                        std::string response;
                        std::cout << "Remove " << (fs::is_directory(expandedPath) ? "directory and its contents" : "file") << " '" << expandedPath << "' (y/n)? ";
                        std::cin >> response;

                        if (response == "y" || response == "Y")
                        {
                            fs::is_directory(expandedPath) ? removeDirectory(expandedPath, verbose) : removeFile(expandedPath, verbose);
                        }
                        else
                        {
                            std::cout << "Operation canceled.\n";
                        }
                    }
                    else
                    {
                        fs::is_directory(expandedPath) ? removeDirectory(expandedPath, verbose) : removeFile(expandedPath, verbose);
                    }
                }
                else
                {
                    std::cerr << "Error: Path does not exist: " << expandedPath << std::endl;
                }
            }
        }
        else
        {
            if (fs::exists(pathToRemove))
            {
                if (fs::is_directory(pathToRemove) && !recursive)
                {
                    std::cout << "rm: cannot remove " << pathToRemove << ": Is a directory\n";
                    return 0;
                }

                if (interactive)
                {
                    std::string response;
                    std::cout << "Remove " << (fs::is_directory(pathToRemove) ? "directory and its contents" : "file") << " '" << pathToRemove << "' (y/n)? ";
                    std::cin >> response;

                    if (response == "y" || response == "Y")
                    {
                        fs::is_directory(pathToRemove) ? removeDirectory(pathToRemove, verbose) : removeFile(pathToRemove, verbose);
                    }
                    else
                    {
                        std::cout << "Operation canceled.\n";
                    }
                }
                else
                {
                    fs::is_directory(pathToRemove) ? removeDirectory(pathToRemove, verbose) : removeFile(pathToRemove, verbose);
                }
            }
            else
            {
                std::cerr << "Error: Path does not exist: " << pathToRemove << std::endl;
                std::exit(EXIT_FAILURE);
            }
        }
    }

    return 0;
}
