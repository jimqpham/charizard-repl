#pragma once

#include "expr.h"

class Add : public Expr {
    public:
        Expr *lhs;
        Expr *rhs;

        Add(Expr* lhs, Expr* rhs);

        virtual bool equals(Expr *o);
};
