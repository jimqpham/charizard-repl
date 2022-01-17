#include "mult.h"

Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
};

bool Mult::equals(Expr *o) {
    Mult *multExpr = dynamic_cast<Mult*>(o);
    if (multExpr == nullptr)
        return false;
    else
        return (multExpr->lhs->equals(this->lhs)) && (multExpr->rhs->equals(this->rhs));
}