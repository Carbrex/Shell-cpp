#include "Shell.h"
#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <unistd.h>
#include <glob.h>

namespace fs = std::filesystem;

struct Command
{
    std::string name;
    std::vector<std::string> options;
};
Command parseCommand(const std::string &userInput)
{
    Command result;

    std::istringstream iss(userInput);
    iss >> result.name; // The first word is assumed to be the command name

    // Parse the remaining words as options
    std::string option;
    while (iss >> option)
    {
        result.options.push_back(option);
    }

    return result;
}

std::string get_pwd()
{
    char buffer[1024]; // Buffer to store the current working directory
    if (getcwd(buffer, sizeof(buffer)) != nullptr)
    {
        return std::string(buffer);
    }
    else
    {
        perror("getcwd");
        return ""; // Return an empty string in case of an error
    }
}

void Shell::executeCommand(const Command &parsedCommand)
{
    // std::command=parsedCommand.name;
    if (parsedCommand.name == "cd")
    {
        executeCd(parsedCommand);
    }
    else if (parsedCommand.name == "mv")
    {
        executeMv(parsedCommand);
    }
    else if (parsedCommand.name == "rm")
    {
        executeRm(parsedCommand);
    }
    else if (parsedCommand.name == "ls")
    {
        executeLs(parsedCommand);
    }
    else if (parsedCommand.name == "cp")
    {
        executeCp(parsedCommand);
    }
    else
    {
        std::cout << "Unknown command: " << parsedCommand.name << std::endl;
    }
}

void Shell::executeCd(const Command &parsedCommand)
{
    if (parsedCommand.options.empty())
    {
        std::cout << "Usage: cd <directory>" << std::endl;
        std::cout << "Usage: cd [options] [directory]\n"
                  << "Change the current directory.\n\n"
                  << "Options:\n"
                  << "  -l      Display the current directory\n"
                  << "  -h      Display this help message\n";
    }
    else
    {
        bool displayCurrentDir = false;
        bool help = false;
        for (auto &&i : parsedCommand.options)
        {
            if (i == "-h")
            {
                help = true;
            }
            else if (i == "-l")
            {
                displayCurrentDir = true;
            }
        }

        if (help)
        {
            std::cout << "Usage: cd [options] [directory]\n"
                      << "Change the current directory.\n\n"
                      << "Options:\n"
                      << "  -l      Display the current directory\n"
                      << "  -h      Display this help message\n";
            return;
        }

        if (displayCurrentDir)
        {
            std::cout << get_pwd() << '\n';
        }
        else
        {
            try
            {
                fs::current_path(parsedCommand.options[0]);
            }

            catch (const std::exception &e)
            {
                std::cerr << "Error changing directory: " << e.what() << std::endl;
            }
        }
    }
}

void Shell::executeMv(const Command &parsedCommand)
{
    if (parsedCommand.options.size() != 2)
    {
        std::cout << "Usage: mv <source> <destination>" << std::endl;
    }
    else
    {
        try
        {
            fs::rename(parsedCommand.options[0], parsedCommand.options[1]);
        }
        catch (const std::exception &e)

        {
            std::cerr << "Error moving file/directory: " << e.what() << std::endl;
        }
    }
}

void Shell::executeRm(const Command &parsedCommand)
{
    if (parsedCommand.options.empty())
    {
        std::cout << "Usage: rm <file/directory>" << std::endl;
    }
    else
    {
        try
        {
            fs::remove_all(parsedCommand.options[0]);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error removing file/directory: " << e.what() << std::endl;
        }
    }
}

void Shell::executeLs(const Command &parsedCommand)
{
    // int opt;
    // bool showHidden = false;
    // bool reverseOrder = false;
    // bool longFormat = false;

    // while ((opt = getopt(argc, argv, "Ral")) != -1)
    // {
    //     switch (opt)
    //     {
    //     case 'a':
    //         showHidden = true;
    //         break;
    //     case 'r':
    //         reverseOrder = true;
    //         break;
    //     case 'l':
    //         longFormat = true;
    //         break;
    //     case '?':
    //         std::cerr << "Unknown option: -" << char(optopt) << std::endl;
    //         return 1;
    //     default:
    //         return 1;
    //     }
    // }

    // // If no directory is provided, use the current working directory
    // const char *path = (optind < argc) ? argv[optind] : ".";

    // // Check if the '-R' option is present
    // if (std::find(argv, argv + argc, std::string("-R")) != argv + argc)
    // {
    //     // listFilesRecursive(path, showHidden, reverseOrder, longFormat);
    // }
    // else
    // {
    //     // Non-recursive version
    //     // listFiles(path, showHidden, reverseOrder, longFormat);
    // }
    // bool recursive = false;
    // bool showHidden = false;

    // // Process options
    // for (const auto &option : parsedCommand.options)
    // {
    //     if (option == "-r" || option == "--recursive")
    //     {
    //         recursive = true;
    //     }
    //     else if (option == "-h" || option == "--hidden")
    //     {
    //         showHidden = true;
    //     }
    //     else if (option == "--help")
    //     {
    //         // Display help for ls command
    //         std::cout << "Usage: ls [OPTION]... [FILE]...\n";
    //         std::cout << "List information about the FILEs (the current directory by default).\n";
    //         std::cout << "\nOptions:\n";
    //         std::cout << "  -r, --recursive   list subdirectories recursively\n";
    //         std::cout << "  -h, --hidden      do not ignore entries starting with .\n";
    //         std::cout << "      --help        display this help and exit\n";
    //         return;
    //     }
    //     // Add more options as needed
    // }

    // // List files and directories based on provided pattern
    // try
    // {
    //     for (const auto &entry : fs::directory_iterator("."))
    //     {
    //         if (!showHidden && entry.path().filename().string()[0] == '.')
    //         {
    //             continue; // Skip hidden files if the option is not specified
    //         }

    //         std::cout << entry.path().filename() << std::endl;

    //         if (recursive && fs::is_directory(entry))
    //         {
    //             // If recursive option is specified, list subdirectories recursively
    //             // You need to implement recursive listing logic here
    //             listFilesRecursively(entry.path(), showHidden);
    //         }
    //     }
    // }
    // catch (const std::exception &e)
    // {
    //     std::cerr << "Error listing directory: " << e.what() << std::endl;
    // }
}

void Shell::listFilesRecursively(const fs::path &directory, bool showHidden)
{
    try
    {
        for (const auto &entry : fs::recursive_directory_iterator(directory))
        {
            if (!showHidden && entry.path().filename().string()[0] == '.')
            {
                continue; // Skip hidden files if the option is not specified
            }

            std::cout << entry.path().filename() << std::endl;
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error listing directory recursively: " << e.what() << std::endl;
    }
}

void Shell::executeCp(const Command &parsedCommand)
{
    if (parsedCommand.options.size() != 2)
    {
        std::cout << "Usage: cp <source> <destination>" << std::endl;
    }
    else
    {
        try
        {
            fs::copy(parsedCommand.options[0], parsedCommand.options[1], fs::copy_options::recursive);
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error copying file/directory: " << e.what() << std::endl;
        }
    }
}

std::vector<std::string> Shell::expandWildcards(const std::string &pattern)
{
    std::vector<std::string> result;
    try
    {
        for (const auto &entry : fs::directory_iterator(pattern))
        {
            result.push_back(entry.path().string());
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error expanding wildcards: " << e.what() << std::endl;
    }
    return result;
}

std::vector<std::string> Shell::expandWildcards(const std::string &pattern)
{
    std::vector<std::string> result;

    glob_t globResult;
    if (glob(pattern.c_str(), GLOB_TILDE, nullptr, &globResult) == 0)
    {
        for (size_t i = 0; i < globResult.gl_pathc; ++i)
        {
            result.push_back(globResult.gl_pathv[i]);
        }

        globfree(&globResult);
    }
    else
    {
        std::cerr << "Error expanding wildcards for pattern: " << pattern << std::endl;
    }

    return result;
}

// std::vector<std::string> Shell::expandWildcards(const std::string &pattern)
// {
//     std::vector<std::string> result;
//     try
//     {
//         for (const auto &entry : fs::directory_iterator(pattern))
//         {
//             result.push_back(entry.path().string());
//         }
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "Error expanding wildcards: " << e.what() << std::endl;
//     }
//     return result;
// }

int main()
{
    Shell myShell;

    while (true)
    {
        std::string userInput;
        // std::vector<std::string> commandAndOptions;

        // Get user input
        std::string USER = getenv("USER");
        std::string NAME = getenv("NAME");
        std::cout << USER << "@" << NAME << ": " << get_pwd() << "$ ";
        std::getline(std::cin, userInput);
        Command parsedCommand = parseCommand(userInput);

        // Display the parsed command and options
        // std::cout << "Command: " << parsedCommand.name << std::endl;
        // std::cout << "Options:";
        // for (const auto &option : parsedCommand.options)
        // {
        //     std::cout << " " << option;
        // }
        // Exit if the user enters 'exit'
        if (userInput == "exit")
        {
            break; // Exit the loop and terminate the program
        }

        // // Tokenize user input into command and options
        // std::istringstream iss(userInput);
        // std::string token;
        // while (iss >> token)
        // {
        //     commandAndOptions.push_back(token);
        // }

        // Execute the command
        if (parsedCommand.name != "")
        {
            myShell.executeCommand(parsedCommand);
        }
        else
        {
            std::cout << "Invalid command. Please try again." << std::endl;
        }
    }

    std::cout << "Exiting the shell. Goodbye!" << std::endl;

    return 0;
}
