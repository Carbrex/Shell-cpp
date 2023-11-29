#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdlib>
#include <dirent.h>
#include <cstring>
#include <fstream>

bool fileExists(const std::string &path)
{
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

bool isDirectory(const std::string &path)
{
    struct stat pathStat;

    // Use stat to get information about the file
    if (stat(path.c_str(), &pathStat) != 0)
    {
        // Error handling, e.g., file not found
        return false;
    }

    // Check if the file is a directory
    return S_ISDIR(pathStat.st_mode);
}

void copyFile(const std::string &source, const std::string &destination, bool interactive, bool verbose)
{
    if (isDirectory(source))
    {
        std::cout << "cp: -r not specified; omitting directory " << source << '\n';
        return;
    }
    if (fileExists(destination) && !isDirectory(destination))
    {
        if (interactive)
        {
            char response;
            std::cout << "Destination file already exists: " << destination << std::endl;
            std::cout << "Do you want to overwrite? (y/n): ";
            std::cin >> response;

            if (response != 'y' && response != 'Y')
            {
                std::cerr << "Copy operation aborted by user." << std::endl;
                return;
            }
        }
    }
    else if (fileExists(destination))
    {
        std::ifstream src(source, std::ios::binary);
        std::string newDest = destination + "/" + source;
        std::ofstream dest(newDest, std::ios::binary);
    }

    std::ifstream src(source, std::ios::binary);
    std::ofstream dest(destination, std::ios::binary);

    dest << src.rdbuf();

    if (verbose)
    {
        std::cout << "Copied: " << source << " -> " << destination << std::endl;
    }
}

void copyDirectoryRecursive(const std::string &source, const std::string &destination, bool interactive, bool verbose)
{
    DIR *dir;
    struct dirent *entry;
    if (!isDirectory(source))
    {
        std::cout << source << ' ' << destination << '\n';
        std::ifstream src(source, std::ios::binary);
        std::ofstream dest(destination, std::ios::binary);
        return;
    }
    if ((dir = opendir(source.c_str())) == nullptr)
    {
        std::cerr << "Error opening directory: " << source << std::endl;
        return;
    }

    while ((entry = readdir(dir)) != nullptr)
    {
        std::string entryName = entry->d_name;

        if (entryName != "." && entryName != "..")
        {
            std::string sourcePath = source + "/" + entryName;
            std::string destPath = destination + "/" + entryName;

            if (entry->d_type == DT_DIR)
            {
                copyDirectoryRecursive(sourcePath, destPath, interactive, verbose);
            }
            else
            {
                copyFile(sourcePath, destPath, interactive, verbose);
            }
        }
    }

    closedir(dir);
}

void displayHelp()
{
    std::cout << "Usage: cp [options] source destination\n"
              << "Copy files or directories.\n\n"
              << "Options:\n"
              << "  -r      Copy directories recursively\n"
              << "  -i      Prompt before overwriting\n"
              << "  -v      Explain what is being done\n"
              << "  -h      Display this help message\n";
}

int main(int argc, char *argv[])
{
    int opt;
    bool recursive = false;
    bool interactive = false;
    bool verbose = false;
    bool help = false;

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
        case 'v':
            verbose = true;
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

    if (help || argc - optind < 2)
    {
        displayHelp();
        return 0;
    }

    std::string destination = argv[argc - 1];
    int destinationInd = argc - 1;

    if (optind < destinationInd)
    {
        for (size_t i = optind; i < destinationInd; i++)
        {
            const std::string source = argv[i];
            if (!fileExists(source))
            {
                std::cerr << "Error: Source file or directory does not exist: " << source << std::endl;
                return 1;
            }

            // Check if the source is a directory and the recursive option is not provided
            if (!recursive && fileExists(source) && source.back() == '/' && source.find('/') != std::string::npos)
            {
                std::cerr << "Error: Use the -r option to copy a directory: " << source << std::endl;
                return 1;
            }
            if (recursive && fileExists(source) && !fileExists(destination))
            {
                if (mkdir(destination.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) != 0)
                {
                    std::cerr << "Error creating destination directory: " << destination << std::endl;
                    return 1;
                }
            }

            std::string realDest = destination;
            if (fileExists(destination) && isDirectory(destination))
            {
                destination = realDest + '/' + source;
            }
            if (recursive && fileExists(source) && fileExists(destination))
            {
                copyDirectoryRecursive(source, destination, interactive, verbose);
            }
            else
            {
                copyFile(source, destination, interactive, verbose);
            }
            destination = realDest;
        }
    }
    return 0;
}
