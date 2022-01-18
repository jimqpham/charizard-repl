cmake_minimum_required(VERSION 3.21)
project(msd_script_charizard)

set(CMAKE_CXX_STANDARD 14)

add_executable(msd_script_charizard main.cpp CmdLines/cmdline.cpp CmdLines/cmdline.h Expressions/expr.cpp Expressions/expr.h Expressions/num.cpp Expressions/num.h Expressions/add.cpp Expressions/add.h Expressions/mult.cpp Expressions/mult.h Expressions/var.cpp Expressions/var.h)
