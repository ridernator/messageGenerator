#include "MessageGenerator.hpp"
#include "Language.hpp"
#include "FileParser.hpp"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <iostream>

MessageGenerator::MessageGenerator(const int argc,
                                   char** argv) : javaRequired(false),
                                                  cppRequired(false),
                                                  verbose(false) {
    int option;
    int option_index = 0;
    std::string outputFolder = ".";

    static struct option long_options[] = {
        {"verbose", no_argument, 0, 'v'},
        {"cpp", no_argument, 0, 'c'},
        {"java", no_argument, 0, 'j'},
        {"outputFolder", required_argument, 0, 'o'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    while ((option = getopt_long(argc, argv, "vcjho:", long_options, &option_index)) != -1) {
        switch (option) {
            case 'v': {
                verbose = true;

                break;
            }

            case 'c': {
                cppRequired = true;

                break;
            }

            case 'j': {
                javaRequired = true;

                break;
            }

            case 'i': {
                puts("option -i\n");

                break;
            }

            case 'o': {
                outputFolder = optarg;

                break;
            }

            case 'h': {
                printUsageAndExit();

                break;
            }

            default: {
                printUsageAndExit();

                break;
            }
        }
    }

    std::vector<std::string> filesToParse;

    while (optind < argc) {
        filesToParse.push_back(argv[optind++]);
    }

    std::cout << "C++            : " << (cppRequired ? "true" : "false") << std::endl;
    std::cout << "Java           : " << (javaRequired ? "true" : "false") << std::endl;
    std::cout << "Verbose        : " << (verbose ? "true" : "false") << std::endl;
    std::cout << "Output Folder  : " << outputFolder << std::endl;
    std::cout << "Files To Parse : ";

    for (const auto& file : filesToParse) {
        std::cout << file << std::endl << "                 ";
    }
    std::cout << '\r';
    
    FileParser parser;

    for (const auto& file : filesToParse) {
        if (cppRequired) {
            parser.parse(file, CPP, outputFolder);
        }
        
        if (javaRequired) {
            parser.parse(file, JAVA, outputFolder);
        }
    }
}

void MessageGenerator::printUsageAndExit() {
    fprintf(stdout, "Usage : \n");

    exit(EXIT_SUCCESS);
}

int main(const int argc,
         char** argv) {
    MessageGenerator messageGenerator(argc, argv);
}

