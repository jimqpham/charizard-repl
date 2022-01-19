#pragma once

#include "expr.h"

class Num : public Expr {
    public:
        int val;

        Num(int val);

        bool equals(Expr *o) override;

        int interp() override;

        bool has_variable() override;

        Expr* subst(std::string stringToMatch, Expr* replcExpr) override;
};


