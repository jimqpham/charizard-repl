#include <iostream>
#include <string>
#include <cstdlib>
#include "exec.h"

// This file demonstrates and stress-tests "exec.cpp"

const int N = 1000000;
const int RANDOM_ITERS = 100;

static void check_success(ExecResult &r);

static std::string make_big_string(std::string word, int size);

static std::string random_bytes();

int main(int argc, const PTR(char)argv[]) {
    std::string big_string = make_big_string("hello", N);

    // Example: running `wc`
    std::cout << "Trying wc\n";
    const PTR(char) const
    wc_argv[] = {"/usr/bin/wc", "-w"};
    ExecResult wc_result = exec_program(2, wc_argv, big_string);
    check_success(wc_result);
    if (std::stoi(wc_result.out) != N)
        std::cerr << "bad wc result\n";

    // Example: checking that `cat` echoes its input
    std::cout << "Trying cat\n";
    const PTR(char) const
    cat_argv[] = {"/bin/cat"};
    ExecResult cat_result = exec_program(1, cat_argv, big_string);
    check_success(cat_result);
    if (big_string != cat_result.out)
        std::cerr << "bad cat result\n";

    // Stress test by checking `cat` even more
    for (int i = 0; i < RANDOM_ITERS; i++) {
        std::cout << "Trying cat " << i << "\n";
        int size = rand() % N;
        std::string word = random_bytes();
        std::string random_big_string = make_big_string(word, size);
        ExecResult random_cat_result = exec_program(1, cat_argv, random_big_string);
        check_success(random_cat_result);
        if (random_big_string != random_cat_result.out) {
            std::cerr << "bad random cat result\n";
            std::cerr << " word: ";
            for (int i = 0; i < word.length(); i++)
                std::cerr << " " << (int) (unsigned char) word[i];
            std::cerr << "\n";
            std::cerr << " word length: " << word.length() << "\n";
            std::cerr << " size: " << size << "\n";
            std::cerr << " input length:  " << random_big_string.length() << "\n";
            std::cerr << " output length: " << random_cat_result.out.length() << "\n";
        }
    }

    return 0;
}

static void check_success(ExecResult &result) {
    std::cerr << result.err;
    if (result.exit_code != 0)
        std::cerr << "non-zero exit: " << result.exit_code << "\n";
}

// Makes `size` copies of `word`, each with a newline afterward
static std::string make_big_string(std::string word, int size) {
    std::string big;
    for (int i = 0; i < size; i++)
        big += word + "\n";
    return big;
}

// Makes a string of up to 32 random bytes
static std::string random_bytes() {
    std::string word = "";
    for (int i = rand() % 32; i-- > 0;) {
        word += rand() % 256;
    }
    return word;
}