#pragma once

#include "../shared_ptr.h"

class Val;

CLASS(Env) {
public:
    virtual PTR(Val) lookup(std::string find_name) = 0;

    static PTR(Env) empty;
};
