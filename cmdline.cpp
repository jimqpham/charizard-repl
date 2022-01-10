#include <iostream>
//#include "cmdline.h"

void use_arguments (int argc, char** argv) {

    // --help: show the manual
    // --test: run the test
    // no help/test in the list of arguments: exit error
    // no arguments: return

    if (argc <= 1)
        return;
    else {
        for (int i = 0; i < argc; i++) {
            if (strcmp(argv[i], "--help") == 0) {
                std::cout << "Allowed arguments:\n";
                std::cout << "\t > --help: Show manual\n";
                std::cout << "\t > --test: Run the test. Test passed when there is only one --test argument\n";
                std::cout << "\t > No argument: Return\n";
                exit(0);
            } else if (strcmp(argv[i], "--test") == 0) {
                for (int j = i + 1; j < argc; j++) {
                    if (strcmp(argv[j], "--test") == 0) {
                        std::cerr << "Tests failed\n";
                        exit(1);
                    }
                }

                std::cout << "Tests passed\n";
                exit(0);
            }
        }

        std::cerr << "Invalid arguments!\n";
        exit(1);
    }
}