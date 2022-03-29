#define CATCH_CONFIG_RUNNER

#include <iostream>
#include <string>
#include "cmdline.h"
#include "parse.h"
#include "../Testing/catch.h"
#include "../Expressions/expr.h"
#include "../Vals/num_val.h"
#include "../Utils/env.h"
#include "../Utils/step.h"

typedef enum {
    INTERP,
    STEP_INTERP,
    PRINT,
    PRETTY_PRINT
} command_t;

void interactiveCommand(command_t command);

void use_arguments(int argc, char **argv) {

    std::string helpText = "Allowed arguments:\n"
                           "\t > --help: Show manual\n"
                           "\t > --test: Run the test. Test passed when there is only one --test argument\n"
                           "\t > --interp: Open the interpreter to evaluate expressions\n"
                           "\t > --step: Open the interpreter in STEP mode to avoid stack overflow\n"
                           "\t > --print: Print out the input with raw formatting\n"
                           "\t > --pretty-print: Print out the input with pretty formatting\n"
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
            } else if (strcmp(argv[i], "--test") == 0) {
                if (!printTestsPassed) {
                    Catch::Session().run(1, argv);
                    printTestsPassed = true;
                } else {
                    throw std::runtime_error("Duplicate --test params!");
                }
            } else if (strcmp(argv[i], "--step") == 0) {
                interactiveCommand(STEP_INTERP);
            } else if (strcmp(argv[i], "--interp") == 0) {
                interactiveCommand(INTERP);
            } else if (strcmp(argv[i], "--print") == 0) {
                interactiveCommand(PRINT);
            } else if (strcmp(argv[i], "--pretty-print") == 0) {
                interactiveCommand(PRETTY_PRINT);
            } else {
                throw std::runtime_error("Invalid command line arguments!");
            }
        }
        return;
    }
}

void interactiveCommand(command_t command) {
    while (true) {
        PTR(Expr) e = parse(std::cin);

        std::string output;
        switch (command) {
            case INTERP:
                output = e->interp()->to_string();
                break;
            case STEP_INTERP:
                output = Step::interp_by_steps(e)->to_string();
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