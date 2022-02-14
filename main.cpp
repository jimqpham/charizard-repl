#include <stdexcept>
#include <iostream>
#include "CmdLines/cmdline.h"

int main(int argc, char **argv) {
    try {
        use_arguments(argc, argv);
        std::cout << "Program exiting. Exit code 0\n";
        return 0;
    }
    catch (std::runtime_error exn) {
        std::cerr << "Error: " << exn.what() << "\n";
        std::cout << "Program exiting with an error. Exit code 1\n";
        return 1;
    }
}
