#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <vector>
#include <algorithm>

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
                std::cout << "\tSize: " << fileStat.st_size << " bytes";
                std::cout << "\tLast modified: " << ctime(&fileStat.st_mtime);
            }
        }

        std::cout << std::endl;
    }
}

void listFilesRecursive(const char *path, bool showHidden, bool reverseOrder, bool longFormat)
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
        std::string filePath = std::string(path) + "/" + fileName;

        std::cout << filePath;

        // Optionally print additional information for long format
        if (longFormat)
        {
            struct stat fileStat;

            if (stat(filePath.c_str(), &fileStat) == 0)
            {
                std::cout << "\tSize: " << fileStat.st_size << " bytes";
                std::cout << "\tLast modified: " << ctime(&fileStat.st_mtime);
            }
        }

        std::cout << std::endl;

        // If the entry is a subdirectory, recursively call listFilesRecursive
        if (entry->d_type == DT_DIR && std::string(fileName) != "." && std::string(fileName) != "..")
        {
            listFilesRecursive(filePath.c_str(), showHidden, reverseOrder, longFormat);
        }
    }
}

int main(int argc, char *argv[])
{
    int opt;
    bool showHidden = false;
    bool reverseOrder = false;
    bool longFormat = false;
    bool recursive = false;

    while ((opt = getopt(argc, argv, "rRal")) != -1)
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
        case '?':
            std::cerr << "Unknown option: -" << char(optopt) << std::endl;
            return 1;
        default:
            return 1;
        }
    }

    // If no directory is provided, use the current working directory
    const char *path = (optind < argc) ? argv[optind] : ".";

    // Check if the '-R' option is present
    if (recursive)
    {
        listFilesRecursive(path, showHidden, reverseOrder, longFormat);
    }
    else
    {
        // Non-recursive version
        listFiles(path, showHidden, reverseOrder, longFormat);
    }

    return 0;
}