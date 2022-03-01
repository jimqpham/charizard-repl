#include "add_expr.h"
#include "../Vals/val.h"

AddExpr::AddExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool AddExpr::equals(Expr *o) {
    AddExpr *addExpr = dynamic_cast<AddExpr *>(o);
    if (addExpr == nullptr)
        return false;
    else
        return (addExpr->lhs->equals(this->lhs)) && (addExpr->rhs->equals(this->rhs));
}

Val *AddExpr::interp() {
    return (this->lhs->interp()->add_to(this->rhs->interp()));
}

Expr *AddExpr::subst(std::string stringToMatch, Expr *replcExpr) {
    Expr *substLhs = this->lhs->subst(stringToMatch, replcExpr);
    Expr *substRhs = this->rhs->subst(stringToMatch, replcExpr);
    AddExpr *result = new AddExpr(substLhs, substRhs);
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