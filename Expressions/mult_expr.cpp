#include "mult_expr.h"
#include "../Vals/val.h"
#include "../Utils/env.h"
#include "../Utils/step.h"
#include "../Utils/add_cont.h"

MultExpr::MultExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool MultExpr::equals(PTR(Expr) o) {
    PTR(MultExpr) multExpr = CAST(MultExpr)(o);
    if (multExpr == nullptr)
        return false;
    else
        return (multExpr->lhs->equals(this->lhs)) && (multExpr->rhs->equals(this->rhs));
}

PTR(Val) MultExpr::interp_env(PTR(Env) env) {
    return (this->lhs->interp_env(env)->mult_by(this->rhs->interp_env(env)));
}

void MultExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = lhs;
    Step::env = Step::env;
    Step::cont = NEW(RightThenAddCont)(rhs, Step::env, Step::cont);
}

void MultExpr::print(std::ostream &out) {
    out << std::string("(");
    this->lhs->print(out);
    out << std::string("*");
    this->rhs->print(out);
    out << std::string(")");
}

void MultExpr::pretty_print_at(std::ostream &out,
                               precedence_t precedence,
                               bool kwrdNeedsPars,
                               std::streampos &newLinePos) {
    bool parenthesized = precedence >= prec_mult;

    out << (parenthesized ? "(" : "");
    this->lhs->pretty_print_at(out, prec_mult, true, newLinePos);
    out << std::string(" * ");
    this->rhs->pretty_print_at(out, prec_add, !parenthesized && kwrdNeedsPars, newLinePos);
    out << (parenthesized ? ")" : "");
}
