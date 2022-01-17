#pragma once

#include "expr.h"

class Num : public Expr {
    public:
        int val;

        Num(int val);

        virtual bool equals(Expr *o);
};


