

#include <string>
#include <iostream>

std::string random_expr_string() {
    if ((rand() % 10) < 6)
        return std::to_string(rand());
    else
        return random_expr_string() + "+" + random_expr_string();
}

int main(int argc, char **argv) {
    const char *const interp1_argv[] = {"msdscript", "--interp"};
    const char *const interp2_argv[] = {"msdscript2", "--interp"};
    for (int i = 0; i < 100; i++) {
        std::string in = random_expr_string();
        std::cout << "Trying " << in << "\n";
        ExecResult interp1_result = exec_program(2, interp1_argv, in);
        ExecResult interp2_result = exec_program(2, interp2_argv, in);
        if (interp1_result.out != interp2_result.out)
            throw std::runtime_error("different results");
    }
    return 0;
}