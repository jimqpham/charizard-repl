#include "cont.h"

class Val;

class Expr;

class Env;

class CallCont : public Cont {
    PTR(Val) to_be_called_val;
    PTR(Cont) rest;

public:
    CallCont(PTR(Val) to_be_called_val, PTR(Cont) rest) :
            to_be_called_val(to_be_called_val), rest(rest) {};

    void step_continue() override;
};

class ArgThenCallCont : public Cont {
    PTR(Expr) actual_arg;
    PTR(Env) env;
    PTR(Cont) rest;

public:
    ArgThenCallCont(PTR(Expr) actual_arg, PTR(Env) env, PTR(Cont) rest) :
            actual_arg(actual_arg), env(env), rest(rest) {};

    void step_continue() override;
};


