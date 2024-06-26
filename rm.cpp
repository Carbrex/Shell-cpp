#include "rm.h"

void RM::removeFile(const std::filesystem::path &filePath, bool verbose)
{
    try
    {
        std::filesystem::remove(filePath);
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

void RM::removeDirectory(const std::filesystem::path &dirPath, bool verbose)
{
    try
    {
        std::filesystem::remove_all(dirPath);
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

std::vector<std::filesystem::path> RM::expandWildcard(const std::filesystem::path &pattern)
{
    std::vector<std::filesystem::path> result;
    for (const auto &entry : std::filesystem::directory_iterator(pattern))
    {
        result.push_back(entry.path());
    }
    return result;
}

int RM::executeRM(int argc, char *argv[])
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

    for (int i = optind; i < argc; i++)
    {
        std::filesystem::path pathToRemove = argv[i];
        if (pathToRemove.has_relative_path() && (pathToRemove.has_filename() && (pathToRemove.filename().string().find('*') != std::string::npos || pathToRemove.filename().string().find('?') != std::string::npos)))
        {
            std::vector<std::filesystem::path> expandedPaths = expandWildcard(pathToRemove);
            for (const auto &expandedPath : expandedPaths)
            {
                if (std::filesystem::exists(expandedPath))
                {
                    if (std::filesystem::is_directory(expandedPath))
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
                        std::cout << "Remove " << (std::filesystem::is_directory(expandedPath) ? "directory and its contents" : "file") << " '" << expandedPath << "' (y/n)? ";
                        std::cin >> response;

                        if (response == "y" || response == "Y")
                        {
                            std::filesystem::is_directory(expandedPath) ? removeDirectory(expandedPath, verbose) : removeFile(expandedPath, verbose);
                        }
                        else
                        {
                            std::cout << "Operation canceled.\n";
                        }
                    }
                    else
                    {
                        std::filesystem::is_directory(expandedPath) ? removeDirectory(expandedPath, verbose) : removeFile(expandedPath, verbose);
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
            if (std::filesystem::exists(pathToRemove))
            {
                if (std::filesystem::is_directory(pathToRemove) && !recursive)
                {
                    std::cout << "rm: cannot remove " << pathToRemove << ": Is a directory\n";
                    return 0;
                }

                if (interactive)
                {
                    std::string response;
                    std::cout << "Remove " << (std::filesystem::is_directory(pathToRemove) ? "directory and its contents" : "file") << " '" << pathToRemove << "' (y/n)? ";
                    std::cin >> response;

                    if (response == "y" || response == "Y")
                    {
                        std::filesystem::is_directory(pathToRemove) ? removeDirectory(pathToRemove, verbose) : removeFile(pathToRemove, verbose);
                    }
                    else
                    {
                        std::cout << "Operation canceled.\n";
                    }
                }
                else
                {
                    std::filesystem::is_directory(pathToRemove) ? removeDirectory(pathToRemove, verbose) : removeFile(pathToRemove, verbose);
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

int main(int argc, char *argv[])
{
    RM rm;
    return rm.executeRM(argc,argv);
}
