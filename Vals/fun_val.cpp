#include "fun_val.h"
#include "../Expressions/fun_expr.h"
#include "../Utils/extended_env.h"
#include "../Utils/step.h"

FunVal::FunVal(std::string formal_arg, PTR(Expr) body, PTR(Env) env) {
    this->formal_arg = formal_arg;
    this->body = body;
    this->env = env;
}

bool FunVal::value_equals(PTR(Val) o) {
    PTR(FunVal) otherVal = CAST(FunVal)(o);

    return otherVal != nullptr &&
           otherVal->formal_arg == this->formal_arg &&
           otherVal->body->equals(this->body);
}

PTR(Val) FunVal::add_to(PTR(Val) o) {
    throw std::runtime_error("add of non-number");
}

PTR(Val) FunVal::mult_by(PTR(Val) o) {
    throw std::runtime_error("mult of non-number");
}

std::string FunVal::to_string() {
    return "[function]";
}

PTR(Val) FunVal::call(PTR(Val) actual_arg) {
    return this->body->interp_env(NEW(ExtendedEnv)(formal_arg, actual_arg, env));
}

void FunVal::call_step(std::shared_ptr<Val> actual_arg_val, std::shared_ptr<Cont> rest) {
    Step::mode = Step::interp_mode;
    Step::expr = body;
    Step::env = NEW(ExtendedEnv)(formal_arg, actual_arg_val, env);
    Step::cont = rest;
}
