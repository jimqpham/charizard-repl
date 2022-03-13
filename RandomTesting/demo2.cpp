#include <iostream>
#include "exec.h"

bool contains(std::string s1, std::string s2);

int main(int argc, const PTR(char)argv[]) {
    for (int i = 0; i < 10; i++) {
        const PTR(char) const
        my_argv[] = {"/tmp/msdscript", "--test"};
        ExecResult result = exec_program(2, my_argv, "");

        if (result.exit_code != 0) {
            std::cerr << "failed\n";
            std::cerr << result.err;
            return 1;
        }
        if (!contains(result.out, "All tests passed")) {
            std::cerr << "bad output\n";
            std::cerr << result.err;
            std::cerr << result.out;
            return 1;
        }
    }

    {
        const PTR(char) const
        my_argv[] = {"/tmp/ms/msdscript3", "--interp"};
        ExecResult result = exec_program(2, my_argv, "1 + 2");
        if (result.out != "3\n") {
            std::cerr << "bad output\n";
            std::cout << result.out;
            std::cerr << result.err;
            return 1;
        }
    }
    std::cout << "All good\n";
    return 0;
}

bool contains(std::string s1, std::string s2) {
    return s1.find(s2) != std::string::npos;
}