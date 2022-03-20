#include "fun_val.h"
#include "../Expressions/fun_expr.h"
#include "../Expressions/extended_env.h"

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
    return this->to_expr()->to_string();
}

PTR(Expr) FunVal::to_expr() {
    return NEW(FunExpr)(this->formal_arg, this->body);
}

PTR(Val) FunVal::call(PTR(Val) actual_arg) {
    return this->body->interp_env(NEW(ExtendedEnv)(formal_arg, actual_arg, env));
}
