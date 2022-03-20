#include "mult_expr.h"
#include "../Vals/val.h"
#include "env.h"

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

PTR(Val)MultExpr::interp_env(PTR(Env) env) {
    return (this->lhs->interp_env(env)->mult_by(this->rhs->interp_env(env)));
}

PTR(Expr)MultExpr::subst(std::string stringToMatch, PTR(Expr) replcExpr) {
    PTR(Expr) substLhs = this->lhs->subst(stringToMatch, replcExpr);
    PTR(Expr) substRhs = this->rhs->subst(stringToMatch, replcExpr);
    PTR(MultExpr) result = NEW(MultExpr)(substLhs, substRhs);
    return result;
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
