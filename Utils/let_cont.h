#include <string>
#include "../shared_ptr.h"
#include "cont.h"

class Expr;

class Env;

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
