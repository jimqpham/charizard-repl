#include <iostream>
#include <string>
#include "cmdline.h"

#define CATCH_CONFIG_RUNNER

#include "../Testing/catch.h"
#include "../Expressions/expr.h"
#include "../Expressions/parse.h"

typedef enum {
    INTERP,
    PRINT,
    PRETTY_PRINT
} command_t;

void interactiveCommand(command_t command);

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
                interactiveCommand(INTERP);
                exit(0);
            } else if (strcmp(argv[i], "--print") == 0) {
                interactiveCommand(PRINT);
                exit(0);
            } else if (strcmp(argv[i], "--pretty-print") == 0) {
                interactiveCommand(PRETTY_PRINT);
                exit(0);
            } else {
                std::cerr << "Invalid arguments! Exit with an error...\n";
                exit(1);
            }
        }
        return;
    }
}

void interactiveCommand(command_t command) {
    while (true) {
        // Expr *e = parse(std::cin); – not working, needs to press Enter repeatedly
        std::string input;
        getline(std::cin, input);
        Expr *e = parse_str(input);

        std::string output;
        switch (command) {
            case INTERP:
                output = std::to_string(e->interp());
                break;
            case PRINT:
                output = e->to_string();
                break;
            case PRETTY_PRINT:
                output = e->to_string(true);
                break;
        }
        std::cout << output << "\n";

        skip_whitespace(std::cin);
        if (std::cin.eof())
            break;
    }
}