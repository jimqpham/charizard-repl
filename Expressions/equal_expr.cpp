#include "equal_expr.h"
#include "../Vals/bool_val.h"

EqualExpr::EqualExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool EqualExpr::equals(Expr *o) {
    EqualExpr *equalExpr = dynamic_cast<EqualExpr *>(o);
    if (equalExpr == nullptr)
        return false;
    else
        return (this->lhs->equals(equalExpr->lhs)) &&
               (this->rhs->equals(equalExpr->rhs));
}

Val *EqualExpr::interp() {
    return new BoolVal(this->lhs->interp()->value_equals(this->rhs->interp()));
}

Expr *EqualExpr::subst(std::string stringToMatch, Expr *replcExpr) {
    Expr *substLhs = this->lhs->subst(stringToMatch, replcExpr);
    Expr *substRhs = this->rhs->subst(stringToMatch, replcExpr);
    EqualExpr *result = new EqualExpr(substLhs, substRhs);
    return result;
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
