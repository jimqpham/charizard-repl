#include <stdexcept>
#include <iostream>
#include "CmdLines/cmdline.h"

int main(int argc, char **argv) {
    try {
        use_arguments(argc, argv);
        return 0;
    }
    catch (std::runtime_error exn) {
        std::cerr << "Error: " << exn.what() << "\n";
        return 1;
    }
}
