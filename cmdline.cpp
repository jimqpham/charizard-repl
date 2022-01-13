#include <iostream>
#include <string>
#include "cmdline.h"

void use_arguments (int argc, char** argv) {

    std::string helpText = "Allowed arguments:\n"
                           "\t > --help: Show manual\n"
                           "\t > --test: Run the test. Test passed when there is only one --test argument\n"
                           "\t > Any other arguments: Throw an error and exit\n"
                           "\t > No argument: Return\n"
                           "Exiting...\n";

    if (argc <= 1)
        return;
    else {
        bool printTestsPassed = false;

        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "--help") == 0) {
                std::cout << helpText;
                exit(0);
            } else if (strcmp(argv[i], "--test") == 0) {
                if (!printTestsPassed) {
                    std::cout << "Tests passed!\n";
                    printTestsPassed = true;
                }
                else {
                    std::cerr << "Duplicate --test params! Exit with an error...\n";
                    exit(1);
                }
            } else {
                std::cerr << "Invalid arguments! Exit with an error...\n";
                exit(1);
            }
        }
        return;
    }
}