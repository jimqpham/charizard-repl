#include "add_expr.h"
#include "../Vals/val.h"

AddExpr::AddExpr(PTR(Expr)lhs, PTR(Expr)rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool AddExpr::equals(PTR(Expr)o) {
    PTR(AddExpr)addExpr = dynamic_cast<PTR(AddExpr) >(o);
    if (addExpr == nullptr)
        return false;
    else
        return (addExpr->lhs->equals(this->lhs)) && (addExpr->rhs->equals(this->rhs));
}

PTR(Val)AddExpr::interp() {
    return (this->lhs->interp()->add_to(this->rhs->interp()));
}

PTR(Expr)AddExpr::subst(std::string stringToMatch, PTR(Expr)replcExpr) {
    PTR(Expr)substLhs = this->lhs->subst(stringToMatch, replcExpr);
    PTR(Expr)substRhs = this->rhs->subst(stringToMatch, replcExpr);
    PTR(AddExpr)result = NEW(AddExpr)(substLhs, substRhs);
    return result;
}

void AddExpr::print(std::ostream &out) {
    out << std::string("(");
    this->lhs->print(out);
    out << std::string("+");
    this->rhs->print(out);
    out << std::string(")");
}

void AddExpr::pretty_print_at(std::ostream &out,
                              precedence_t precedence,
                              bool kwrdNeedsPars,
                              std::streampos &newLinePos) {

    bool parenthesized = precedence >= prec_add;

    out << (parenthesized ? "(" : "");
    this->lhs->pretty_print_at(out, prec_add, true, newLinePos);
    out << std::string(" + ");
    this->rhs->pretty_print_at(out, prec_equal, !parenthesized && kwrdNeedsPars, newLinePos);
    out << (parenthesized ? ")" : "");
}