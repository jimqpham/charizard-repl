#pragma once

#include "../shared_ptr.h"

class Expr;

class Cont;

CLASS(Val) {
public:
    virtual bool value_equals(PTR(Val) o) = 0;

    virtual PTR(Val) add_to(PTR(Val) o) = 0;

    virtual PTR(Val) mult_by(PTR(Val) o) = 0;

    virtual std::string to_string() = 0;

    virtual PTR(Val) call(PTR(Val) actual_arg) = 0;

    virtual void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) = 0;
};
