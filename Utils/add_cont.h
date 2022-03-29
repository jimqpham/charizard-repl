#include "../shared_ptr.h"
#include "cont.h"

class Expr;

class Env;

class Val;

class RightThenAddCont : public Cont {
private:
    PTR(Expr) rhs;
    PTR(Env) env;
    PTR(Cont) rest;

public:
    RightThenAddCont(PTR(Expr) rhs, PTR(Env) env, PTR(Cont) rest) : rhs(rhs), env(env), rest(rest) {};

    void step_continue() override;
};

class AddCont : public Cont {
private:
    PTR(Val) lhs_val;
    PTR(Cont) rest;

public:
    AddCont(PTR(Val) lhs_val, PTR(Cont) rest) : lhs_val(lhs_val), rest(rest) {};

    void step_continue() override;
};
