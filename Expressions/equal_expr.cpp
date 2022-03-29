#include "equal_expr.h"
#include "../Vals/bool_val.h"
#include "../Utils/step.h"
#include "../Utils/cont.h"

EqualExpr::EqualExpr(PTR(Expr) lhs, PTR(Expr) rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool EqualExpr::equals(PTR(Expr) o) {
    PTR(EqualExpr) equalExpr = CAST(EqualExpr)(o);
    if (equalExpr == nullptr)
        return false;
    else
        return (this->lhs->equals(equalExpr->lhs)) &&
               (this->rhs->equals(equalExpr->rhs));
}

PTR(Val)EqualExpr::interp_env(PTR(Env) env) {
    return NEW(BoolVal)(this->lhs->interp_env(env)->value_equals(this->rhs->interp_env(env)));
}

void EqualExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = lhs;
    Step::env = Step::env;
    Step::cont = NEW(RightThenCompCont)(rhs, Step::env, Step::cont);
}

void EqualExpr::print(std::ostream &out) {
    out << std::string("(");
    this->lhs->print(out);
    out << std::string("==");
    this->rhs->print(out);
    out << std::string(")");
}

void EqualExpr::pretty_print_at(std::ostream &out, precedence_t precedence, bool kwrdNeedsPars,
                                std::streampos &newLinePos) {

    bool parenthesized = precedence >= prec_equal;

    out << (parenthesized ? "(" : "");
    this->lhs->pretty_print_at(out, prec_equal, true, newLinePos);
    out << std::string(" == ");
    this->rhs->pretty_print_at(out, prec_none, !parenthesized && kwrdNeedsPars, newLinePos);
    out << (parenthesized ? ")" : "");
}

