#include "fun_val.h"
#include "../Expressions/fun_expr.h"

FunVal::FunVal(std::string formal_arg, Expr *body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunVal::value_equals(Val *o) {
    FunVal *otherVal = dynamic_cast<FunVal *>(o);

    return otherVal != nullptr &&
           otherVal->formal_arg == this->formal_arg &&
           otherVal->body->equals(this->body);
}

Val *FunVal::add_to(Val *o) {
    throw std::runtime_error("add of non-number");
}

Val *FunVal::mult_by(Val *o) {
    throw std::runtime_error("mult of non-number");
}

std::string FunVal::to_string() {
    return this->to_expr()->to_string();
}

Expr *FunVal::to_expr() {
    return new FunExpr(this->formal_arg, this->body);
}

Val *FunVal::call(Val *actual_arg) {
    return this->body->subst(this->formal_arg, actual_arg->to_expr())->interp();
}
