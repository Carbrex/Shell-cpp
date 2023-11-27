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
    if (parsedCommand.options.empty())
    {
        // List files and directories in the current directory
        try
        {
            for (const auto &entry : fs::directory_iterator("."))
            {
                std::cout << entry.path().filename() << std::endl;
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error listing directory: " << e.what() << std::endl;
        }
    }
    else
    {
        // List files and directories based on provided pattern
        std::vector<std::string> files = expandWildcards(parsedCommand.options[0]);
        for (const auto &file : files)
        {
            std::cout << file << std::endl;
        }
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
