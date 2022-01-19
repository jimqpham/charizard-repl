#include "add.h"

Add::Add(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool Add::equals(Expr *o) {
    Add* addExpr = dynamic_cast<Add*>(o);
    if (addExpr == nullptr)
        return false;
    else
        return (addExpr->lhs->equals(this->lhs)) && (addExpr->rhs->equals(this->rhs));
}

int Add::interp() {
    return (this->lhs->interp() + this->rhs->interp());
}

bool Add::has_variable() {
    return (this->lhs->has_variable() || this->rhs->has_variable());
}

Expr* Add::subst(std::string stringToMatch, Expr *replcExpr) {
    Expr* substLhs = this->lhs->subst(stringToMatch, replcExpr);
    Expr* substRhs = this->rhs->subst(stringToMatch, replcExpr);
    Add* result = new Add(substLhs, substRhs);
    return result;
}