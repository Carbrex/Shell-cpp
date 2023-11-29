#include "Shell.h"
#include <iostream>
#include <string>
#include <regex>
#include <sstream>
#include <unistd.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <pthread.h>
#include <dirent.h>
#include <cassert>

namespace fs = std::filesystem;

struct Command
{
    std::string name;
    std::vector<std::string> options;
};
Command Shell::parseCommand(const std::string &userInput)
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
    std::vector<std::string> expandedOptions;
    for (const auto &option : result.options)
    {
        if (isWildcard(option.c_str()))
        {
            auto expanded = expandWildcards(option);
            expandedOptions.insert(expandedOptions.end(), expanded.begin(), expanded.end());
        }
        else
        {
            expandedOptions.push_back(option);
        }
    }
    result.options.swap(expandedOptions);
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
                  << "  -v      Explain what is being done\n"
                  << "  -i      Interactive\n"
                  << "  -h      Display this help message\n";
    }
    else
    {
        bool displayCurrentDir = false;
        bool help = false;
        bool interactive = false;
        bool verbose = false;
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
            else if (i == "-i")
            {
                interactive = true;
            }
            else if (i == "-v")
            {
                verbose = true;
            }
        }

        if (help)
        {
            std::cout << "Usage: cd [options] [directory]\n"
                      << "Change the current directory.\n\n"
                      << "Options:\n"
                      << "  -l      Display the current directory\n"
                      << "  -v      Explain what is being done\n"
                      << "  -i      Interactive\n"
                      << "  -h      Display this help message\n";
            return;
        }

        if (displayCurrentDir)
        {
            std::cout << get_pwd() << '\n';
        }
        else
        {
            if (interactive)
            {
                std::cout << "Are you sure you want to cd to " << parsedCommand.options.back() << "(y/n): " << std::endl;
                char response;
                // std::cin.clear();
                // std::cin.ignore();
                std::cin >> response;

                std::cin.clear();
                std::cin.ignore();
                if (response != 'y' && response != 'Y')
                {
                    std::cerr << "cd operation aborted by user." << std::endl;
                    return;
                }
            }

            try
            {
                fs::current_path(parsedCommand.options[parsedCommand.options.size() - 1]);
                if (verbose)
                {
                    std::cout << "Successfully changed directory to " << parsedCommand.options.back() << '\n';
                }
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
    char *argv[parsedCommand.options.size() + 2];
    // Populate the array
    std::string file = "./mv.o";
    argv[0] = new char[file.size() + 1];
    std::strcpy(argv[0], file.c_str());
    for (size_t i = 1; i <= parsedCommand.options.size(); ++i)
    {
        argv[i] = new char[parsedCommand.options[i - 1].size() + 1]; // +1 for null terminator
        std::strcpy(argv[i], parsedCommand.options[i - 1].c_str());
    }
    argv[parsedCommand.options.size() + 1] = nullptr;
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        std::cout << execv("/home/geetika/OOPD/Shell-Project/mv.o", argv);
        // If execv fails, print an error message and exit the child process
        perror("execv");
        exit(EXIT_FAILURE);
    }
    else if (pid == -1)
    {
        printf("Fork Error!");
    }
    else
    {
        waitpid(pid, NULL, 0);
    }
    // Remember to free the allocated memory
    for (size_t i = 0; i < parsedCommand.options.size() + 1; ++i)
    {
        delete[] argv[i];
    }
}

void Shell::executeRm(const Command &parsedCommand)
{
    char *argv[parsedCommand.options.size() + 2];
    // Populate the array
    std::string file = "./rm.o";
    argv[0] = new char[file.size() + 1];
    std::strcpy(argv[0], file.c_str());
    for (size_t i = 1; i <= parsedCommand.options.size(); ++i)
    {
        argv[i] = new char[parsedCommand.options[i - 1].size() + 1]; // +1 for null terminator
        std::strcpy(argv[i], parsedCommand.options[i - 1].c_str());
    }
    argv[parsedCommand.options.size() + 1] = nullptr;
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        std::cout << execv("/home/geetika/OOPD/Shell-Project/rm.o", argv);
        // If execv fails, print an error message and exit the child process
        perror("execv");
        exit(EXIT_FAILURE);
    }
    else if (pid == -1)
    {
        printf("Fork Error!");
    }
    else
    {
        waitpid(pid, NULL, 0);
    }
    // Remember to free the allocated memory
    for (size_t i = 0; i < parsedCommand.options.size() + 1; ++i)
    {
        delete[] argv[i];
    }
}

void Shell::executeLs(const Command &parsedCommand)
{
    char *argv[parsedCommand.options.size() + 2];
    // Populate the array
    std::string file = "./ls.o";
    argv[0] = new char[file.size() + 1];
    std::strcpy(argv[0], file.c_str());
    for (size_t i = 1; i <= parsedCommand.options.size(); ++i)
    {
        argv[i] = new char[parsedCommand.options[i - 1].size() + 1]; // +1 for null terminator
        std::strcpy(argv[i], parsedCommand.options[i - 1].c_str());
    }
    argv[parsedCommand.options.size() + 1] = nullptr;
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        std::cout << execv("/home/geetika/OOPD/Shell-Project/ls.o", argv);
        // If execv fails, print an error message and exit the child process
        perror("execv");
        exit(EXIT_FAILURE);
    }
    else if (pid == -1)
    {
        printf("Fork Error!");
    }
    else
    {
        waitpid(pid, NULL, 0);
    }
    // Remember to free the allocated memory
    for (size_t i = 0; i < parsedCommand.options.size() + 1; ++i)
    {
        delete[] argv[i];
    }
}

void Shell::executeCp(const Command &parsedCommand)
{
    char *argv[parsedCommand.options.size() + 2];
    // Populate the array
    std::string file = "./cp.o";
    argv[0] = new char[file.size() + 1];
    std::strcpy(argv[0], file.c_str());
    for (size_t i = 1; i <= parsedCommand.options.size(); ++i)
    {
        argv[i] = new char[parsedCommand.options[i - 1].size() + 1]; // +1 for null terminator
        std::strcpy(argv[i], parsedCommand.options[i - 1].c_str());
    }
    argv[parsedCommand.options.size() + 1] = nullptr;
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        std::cout << execv("/home/geetika/OOPD/Shell-Project/cp.o", argv);
        // If execv fails, print an error message and exit the child process
        perror("execv");
        exit(EXIT_FAILURE);
    }
    else if (pid == -1)
    {
        printf("Fork Error!");
    }
    else
    {
        waitpid(pid, NULL, 0);
    }
    // Remember to free the allocated memory
    for (size_t i = 0; i < parsedCommand.options.size() + 1; ++i)
    {
        delete[] argv[i];
    }
}

bool Shell::isWildcard(const char *path)
{
    return std::strchr(path, '*') != nullptr || std::strchr(path, '?') != nullptr;
}

std::vector<std::string> Shell::expandWildcards(const std::string &pattern)
{
    std::vector<std::string> result;

    glob_t globResult;
    if (glob(pattern.c_str(), GLOB_TILDE, nullptr, &globResult) == 0)
    {
        for (size_t i = 0; i < globResult.gl_pathc; ++i)
        {
            result.emplace_back(globResult.gl_pathv[i]);
        }

        globfree(&globResult);
    }

    return result;
}

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
        Command parsedCommand = myShell.parseCommand(userInput);

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
