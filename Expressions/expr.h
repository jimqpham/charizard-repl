#pragma once

#include <string>

class Expr {
public:
    virtual bool equals(Expr *o) = 0;

    virtual int interp() = 0;

    virtual bool has_variable() = 0;

    virtual Expr* subst(std::string stringToMatch, Expr* replcExpr) = 0;
};
