#include "mv.h"

bool MV::fileExists(const std::string &path) {
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

void MV::moveFile(const std::string &source, const std::string &destination, bool verbose) {
    // struct stat destStat;

    if (fs::is_directory(destination)) {
        std::string destDir = destination;
        if (destDir.back() != '/') {
            destDir += '/';
        }
        std::string destFile = destDir + fs::path(source).filename().string();

        if (rename(source.c_str(), destFile.c_str()) == 0) {
            if (verbose) {
                std::cout << "Moved: " << source << " -> " << destFile << std::endl;
            }
        } else {
            std::cerr << "Error moving: " << source << " to " << destFile << std::endl;
        }
    } else {
        if (rename(source.c_str(), destination.c_str()) == 0) {
            if (verbose) {
                std::cout << "Moved: " << source << " -> " << destination << std::endl;
            }
        } else {
            std::cerr << "Error moving: " << source << " to " << destination << std::endl;
        }
    }
}

// void MV::moveDirectoryInteractive(const std::string &source, const std::string &destination, bool verbose) {
//     moveFile(source, destination, verbose);
// }

void MV::displayHelp() {
    std::cout << "Usage: mv [options] source destination\n"
              << "Move (rename) files or directories.\n\n"
              << "Options:\n"
              << "  -f      Do not prompt before overwriting\n"
              << "  -v      Explain what is being done\n"
              << "  -i      Prompt before overwriting\n"
              << "  -h      Display this help message\n";
}

int MV::executeMV(int argc, char *argv[]) {
    int opt;
    bool forceMove = false;
    bool verbose = false;
    bool interactive = false;
    bool help = false;

    while ((opt = getopt(argc, argv, "fivh")) != -1) {
        switch (opt) {
        case 'f':
            forceMove = true;
            break;
        case 'v':
            verbose = true;
            break;
        case 'i':
            interactive = true;
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

    if (help || argc - optind < 2) {
        displayHelp();
        return 0;
    }

    const std::string destination = argv[argc - 1];
    int destinationInd = argc - 1;

    if (optind < destinationInd) {
        for (int i = optind; i < destinationInd; i++) {
            const std::string source = argv[i];
            if (!fileExists(source)) {
                std::cerr << "Error: Source file or directory does not exist: " << source << std::endl;
                return 1;
            }

            if (!forceMove && fileExists(destination) && !fs::is_directory(destination)) {
                if (interactive) {
                    char response;
                    std::cout << "Destination file or directory already exists: " << destination << std::endl;
                    std::cout << "Do you want to overwrite? (y/n): ";
                    std::cin >> response;

                    if (response != 'y' && response != 'Y') {
                        std::cerr << "Move operation aborted by user." << std::endl;
                        continue;
                    }
                } else {
                    std::cerr << "Error: Destination file or directory already exists: " << destination << std::endl;
                    continue;
                }
            }

            moveFile(source, destination, verbose);
        }
    }
    return 0;
}


int main(int argc, char *argv[])
{
    MV mv;
    // std::cout<<"I was here\n";
    // for (int i = 0; i < argc; i++)
    // {
    //     std::cout<<argv[i];
    // }
    
    return mv.executeMV(argc,argv);
}