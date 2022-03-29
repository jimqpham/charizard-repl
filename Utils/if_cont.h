#include "cont.h"

class Expr;

class Env;

class IfBranchCont : public Cont {
    PTR(Expr) then_part;
    PTR(Expr) else_part;
    PTR(Env) env;
    PTR(Cont) rest;

public:
    IfBranchCont(PTR(Expr) then_part, PTR(Expr) else_part,
                 PTR(Env) env, PTR(Cont) rest)
            : then_part(then_part), else_part(else_part), env(env), rest(rest) {};

    void step_continue() override;
};