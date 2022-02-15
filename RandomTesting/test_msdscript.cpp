#include "exec.h"
#include <string>
#include <iostream>

// Type of expressions to generate randomly
// Distinguishing between expressions with vars and expressions with no var
// is because expressions with vars are used for (pretty-)print tests
// and expressions with no vars are used for interp tests
typedef enum {
    NUM,
    ADD_WITH_VAR,
    ADD_NO_VAR,
    MULT_WITH_VAR,
    MULT_NO_VAR,
    VAR,
    LET,
    RANDOM_WITH_VAR,
    RANDOM_NO_VAR
} expr_type_t;

// Make a random var with < 32 chars in name
static std::string make_var();

// Randomly generate the expression string based on the type passed as param
std::string expr_string(expr_type_t expr_type);

// Test when there's only a single msdscript program
void test_single(const char *msdscript_path, const char *operation);

std::string exec_res_to_string(ExecResult exec_res);

// Compare outputs of two msdscripts
void compare_output(const char *msds1_path, const char *msds2_path, const char *operation);

int main(int argc, char **argv) {

    if (argc <= 1) {
        std::cerr << "No msdscript path detected. Exiting...\n";
        exit(1);
    } else if (argc == 2) {
        test_single(argv[1], "--interp");
        test_single(argv[1], "--print");
        test_single(argv[1], "--pretty-print");

        std::cout << "\nAll tests passed!";
    } else {
        compare_output(argv[1], argv[2], "--interp");
        compare_output(argv[1], argv[2], "--print");
        compare_output(argv[1], argv[2], "--pretty-print");

        std::cout << "\nAll tests passed!";
    }
}

// Make a random var with < 32 chars in name
static std::string make_var() {
    std::string var = "a"; // make sure var is not empty
    for (int i = rand() % 32; i-- > 0;) {
        var += ('a' + rand() % 26);
    }
    return var;
}

// Randomly generate the expression string based on the type passed as param
std::string expr_string(expr_type_t expr_type) {
    int r = rand() % 10;

    switch (expr_type) {
        case NUM:
            return std::to_string(rand() % 50);
        case ADD_WITH_VAR:
            return "(" + expr_string(RANDOM_WITH_VAR) + "+" + expr_string(RANDOM_WITH_VAR) + ")";
        case ADD_NO_VAR:
            return "(" + expr_string(RANDOM_NO_VAR) + "+" + expr_string(RANDOM_NO_VAR) + ")";
        case MULT_WITH_VAR:
            return expr_string(RANDOM_WITH_VAR) + "*" + expr_string(RANDOM_WITH_VAR);
        case MULT_NO_VAR:
            return expr_string(RANDOM_NO_VAR) + "*" + expr_string(RANDOM_NO_VAR);
        case LET:
            return "(_let " + make_var() + " = " + expr_string(RANDOM_WITH_VAR)
                   + " _in " + expr_string(RANDOM_WITH_VAR) + ")";
        case VAR:
            return make_var();
        case RANDOM_WITH_VAR:
            if (r < 4)
                return expr_string(NUM);
            else if (r < 7)
                return expr_string(VAR);
            else if (r == 7)
                return expr_string(ADD_WITH_VAR);
            else if (r == 8)
                return expr_string(MULT_WITH_VAR);
            else
                return expr_string(LET);
        case RANDOM_NO_VAR:
            if (r < 6)
                return expr_string(NUM);
            else if (r < 8)
                return expr_string(ADD_NO_VAR);
            else
                return expr_string(MULT_NO_VAR);
    }
}

// Test when there's only a single msdscript program
void test_single(const char *msdscript_path, const char *operation) {
    const char *const argv[] = {msdscript_path, operation};
    for (int i = 0; i < 100; i++) {
        // If interp, use no-var expressions. If print/pretty-print, use expressions with vars
        std::string in = strcmp(operation, "--interp") == 0
                         ? expr_string(RANDOM_NO_VAR)
                         : expr_string(RANDOM_WITH_VAR);

        ExecResult result = exec_program(2, argv, in);

        if (result.exit_code != 0) {
            std::cerr << "bad " << operation << " output\n";
            std::cerr << "input: " << in << "\n";
            std::cerr << "error: " << result.err;
            exit(1);
        }

        // Additional test for print:
        // Taking the print output and pass it again through msdscript
        // and make sure there's no mismatch between two prints
        // Skip this test for pretty_print because msdscript does not handle line breaks well
        if (strcmp(operation, "--print") == 0) {
            ExecResult reprint_result = exec_program(2, argv, result.out);
            if (reprint_result.exit_code != 0) {
                std::cerr << "bad reprint output\n" << "initial print output: " << in << "\n" << reprint_result.err;
                exit(1);
            }
            if (strcmp(reprint_result.out.c_str(), result.out.c_str()) != 0) {
                std::cerr << "inconsistencies between print and reprint\n";
                std::cerr << "print output: " << result.out << "\n";
                std::cerr << "reprint output: " << reprint_result.out << "\n";
                exit(1);
            }
        }
    }
}

std::string exec_res_to_string(ExecResult exec_res) {
    std::string str = "";
    str += "Exit code: ";
    str += exec_res.exit_code;
    str += "\n";
    str += "Output: \n" + exec_res.out + "\n";
    str += "Error: " + exec_res.err + "\n";

    return str;
}

// Compare outputs of two msdscripts
void compare_output(const char *msds1_path, const char *msds2_path, const char *operation) {
    const char *const interp1_argv[] = {msds1_path, operation};
    const char *const interp2_argv[] = {msds2_path, operation};
    for (int i = 0; i < 100; i++) {
        // If interp, use no-var expressions. If print/pretty-print, use expressions with vars
        std::string in = strcmp(operation, "--interp") == 0
                         ? expr_string(RANDOM_NO_VAR)
                         : expr_string(RANDOM_WITH_VAR);

        ExecResult result1 = exec_program(2, interp1_argv, in);
        ExecResult result2 = exec_program(2, interp2_argv, in);

        if (strcmp(result1.out.c_str(), result2.out.c_str()) != 0) {
            std::cerr << operation << " mismatch\n";
            std::cerr << "input: " << in << "\n";
            std::cerr << "result from msdscript1: \n" << exec_res_to_string(result1);
            std::cerr << "result from msdscript2: \n" << exec_res_to_string(result2);
            exit(1);
        }
    }
}

