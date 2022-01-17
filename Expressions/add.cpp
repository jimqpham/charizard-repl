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
