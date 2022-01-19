#pragma once

#include "expr.h"

class Mult : public Expr {
    public:
        Expr *lhs;
        Expr *rhs;

        Mult(Expr *lhs, Expr *rhs);

        bool equals(Expr *o) override;

        int interp() override;

        bool has_variable() override;

        Expr* subst(std::string stringToMatch, Expr* replcExpr) override;
};