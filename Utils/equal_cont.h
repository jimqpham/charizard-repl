#include "../shared_ptr.h"
#include "cont.h"

class Expr;

class Env;

class Val;

class RightThenCompCont : public Cont {
private:
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;

public:
    RightThenCompCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest) : rhs(rhs), env(env), rest(rest) {};

    void step_continue() override;
};

class CompCont : public Cont {
private:
    PTR(Val) lhs_val;
    PTR(Cont) rest;

public:
    CompCont(PTR(Val) lhs_val, PTR(Cont) rest) : lhs_val(lhs_val), rest(rest) {};

    void step_continue() override;
};