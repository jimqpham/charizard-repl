#include "cont.h"

class Expr;

class Env;

class Val;

class RightThenMultCont : public Cont {
private:
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;

public:
    RightThenMultCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest) : rhs(rhs), env(env), rest(rest) {};

    void step_continue() override;
};

class MultCont : public Cont {
private:
    PTR(Val) lhs_val;
    PTR(Cont) rest;

public:
    MultCont(PTR(Val) lhs_val, PTR(Cont) rest) : lhs_val(lhs_val), rest(rest) {};

    void step_continue() override;
};