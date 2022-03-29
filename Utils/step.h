#pragma once

#include "../shared_ptr.h"

class Expr;

class Env;

class Val;

class Cont;

class Step {
public:
    typedef enum {
        interp_mode,
        continue_mode,
    } mode_t;

    static mode_t mode;     // chooses mode

    static PTR(Expr) expr;  // interp_mode
    static PTR(Env) env;    // interp_mode

    static PTR(Val) val;    // continue_mode

    static PTR(Cont) cont;  // all modes

    static PTR(Val) interp_by_steps(PTR(Expr) e);
};
