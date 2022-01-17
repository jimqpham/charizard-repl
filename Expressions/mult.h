#pragma once

#include "expr.h"

class Mult : public Expr {
    public:
        Expr *lhs;
        Expr *rhs;

        Mult(Expr *lhs, Expr *rhs);

        virtual bool equals(Expr *o);
};