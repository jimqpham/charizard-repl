#pragma once

#include <string>
#include "../shared_ptr.h"

class Expr;

class Env;

class Val;


CLASS(Cont) {
public:
    static PTR(Cont) done;

    virtual void step_continue() = 0;
};

class DoneCont : public Cont {
    void step_continue() override;
};

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

class LetBodyCont : public Cont {
    std::string var;
    PTR(Expr) body;
    PTR(Env) env;
    PTR(Cont) rest;

public:
    LetBodyCont(std::string var, PTR(Expr) body, PTR(Env) env, PTR(Cont) rest) :
            var(var), body(body), env(env), rest(rest) {};

    void step_continue() override;
};

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

