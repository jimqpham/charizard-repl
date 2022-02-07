#include <iostream>
#include <string>
#include "cmdline.h"

#define CATCH_CONFIG_RUNNER

#include "../Testing/catch.h"

void use_arguments(int argc, char **argv) {

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
                    if (Catch::Session().run(1, argv) != 0)
                        exit(1);
                    printTestsPassed = true;
                } else {
                    std::cerr << "Duplicate --test params! Exit with an error...\n";
                    exit(1);
                }
            } else if (strcmp(argv[i], "--interp") == 0) {
                /* TODO: PARSING */
                std::cout << "Parsing expressions...\n";
                exit(0);
            } else if (strcmp(argv[i], "--print") == 0) {
                /* TODO: PRINTING */
                std::cout << "Printing expressions...\n";
                exit(0);
            } else if (strcmp(argv[i], "--pretty-print") == 0) {
                /* TODO: PRETTY PRINTING */
                std::cout << "Pretty printing expressions...\n";
                exit(0);
            } else {
                std::cerr << "Invalid arguments! Exit with an error...\n";
                exit(1);
            }
        }
        return;
    }
}