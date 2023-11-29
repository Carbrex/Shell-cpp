#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <algorithm>
#include <glob.h>
#include <cstring>
#include <filesystem>

bool fileExists(const std::string &path)
{
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

void listFiles(const char *path, bool showHidden, bool reverseOrder, bool longFormat)
{
    DIR *dir;
    struct dirent *entry;
    std::vector<std::string> fileNames;

    if ((dir = opendir(path)) == nullptr)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != nullptr)
    {
        // Skip hidden files if not specified with -a option
        if (!showHidden && entry->d_name[0] == '.')
            continue;

        fileNames.push_back(entry->d_name);
    }

    closedir(dir);

    // Sort the file names
    if (reverseOrder)
    {
        std::sort(fileNames.rbegin(), fileNames.rend());
    }
    else
    {
        std::sort(fileNames.begin(), fileNames.end());
    }

    for (const auto &fileName : fileNames)
    {
        std::cout << fileName;

        // Optionally print additional information for long format
        if (longFormat)
        {
            struct stat fileStat;
            std::string filePath = std::string(path) + "/" + fileName;

            if (stat(filePath.c_str(), &fileStat) == 0)
            {
                std::cout << "\nSize: " << fileStat.st_size << " bytes";
                std::cout << "\tLast modified: " << ctime(&fileStat.st_mtime);
            }
        }

        std::cout << std::endl;
    }
}

void listFilesRecursive(const char *path, bool showHidden, bool reverseOrder, bool longFormat)
{
    // std::cout<<path<<":\n";
    DIR *dir;
    struct dirent *entry;
    std::vector<std::string> fileNames;

    if ((dir = opendir(path)) == nullptr)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != nullptr)
    {
        // Skip hidden files if not specified with -a option
        if (!showHidden && entry->d_name[0] == '.')
            continue;

        fileNames.push_back(entry->d_name);
    }

    closedir(dir);

    // Sort the file names
    if (reverseOrder)
    {
        std::sort(fileNames.rbegin(), fileNames.rend());
    }
    else
    {
        std::sort(fileNames.begin(), fileNames.end());
    }

    for (const auto &fileName : fileNames)
    {
        std::string filePath = std::string(path) + "/" + fileName;

        std::cout << filePath;

        // Optionally print additional information for long format
        if (longFormat)
        {
            struct stat fileStat;

            if (stat(filePath.c_str(), &fileStat) == 0)
            {
                std::cout << "\nSize: " << fileStat.st_size << " bytes";
                std::cout << "\tLast modified: " << ctime(&fileStat.st_mtime);
            }
        }

        std::cout << std::endl;

        // If the entry is a subdirectory, recursively call listFilesRecursive
        struct stat subDirStat;
        if (stat(filePath.c_str(), &subDirStat) == 0 && S_ISDIR(subDirStat.st_mode))
        {
            listFilesRecursive(filePath.c_str(), showHidden, reverseOrder, longFormat);
        }
    }
}

void displayHelp()
{
    std::cout << "Usage: ls [options] [directory]\n"
              << "List information about files.\n\n"
              << "Options:\n"
              << "  -a      Include entries whose names begin with a dot (.)\n"
              << "  -r      Reverse the order of the sort\n"
              << "  -l      Use a long listing format\n"
              << "  -R      List subdirectories recursively\n"
              << "  -h      Display this help message\n";
}

bool isWildcard(const char *path)
{
    return std::strchr(path, '*') != nullptr || std::strchr(path, '?') != nullptr;
}

int main(int argc, char *argv[])
{
    int opt;
    bool showHidden = false;
    bool reverseOrder = false;
    bool longFormat = false;
    bool recursive = false;
    bool help = false;

    while ((opt = getopt(argc, argv, "hrRal")) != -1)
    {
        switch (opt)
        {
        case 'a':
            showHidden = true;
            break;
        case 'r':
            reverseOrder = true;
            break;
        case 'l':
            longFormat = true;
            break;
        case 'R':
            recursive = true;
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

    // If no directory is provided, use the current working directory
    if (help)
    {
        displayHelp();
        return 0;
    }
    if (optind < argc)
    {
        for (size_t i = optind; i < argc; i++)
        {
            const char *path = argv[i];
            // Check if the '-R' option is present
            if (fileExists(path) && !std::filesystem::is_directory(path))
            {
                std::cout << path << '\n';
            }
            else if (recursive)
            {
                listFilesRecursive(path, showHidden, reverseOrder, longFormat);
            }
            else
            {
                std::cout<<path<<":\n";
                listFiles(path, showHidden, reverseOrder, longFormat);
                std::cout<<'\n';
            }
        }
    }
    else
    {
        const char *path = ".";
        if (recursive)
        {
            listFilesRecursive(path, showHidden, reverseOrder, longFormat);
        }
        else
        {
            listFiles(path, showHidden, reverseOrder, longFormat);
        }
    }

    return 0;
}