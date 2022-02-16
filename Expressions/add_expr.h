#pragma once

#include "expr.h"
#include <iostream>

class AddExpr : public Expr {
public:
    Expr *lhs;
    Expr *rhs;

    AddExpr(Expr *lhs, Expr *rhs);

    bool equals(Expr *o) override;

    int interp() override;

    bool has_variable() override;

    Expr *subst(std::string stringToMatch, Expr *replcExpr) override;

    void print(std::ostream &out) override;

    void pretty_print_at(std::ostream &out,
                         precedence_t precedence,
                         bool needsParenthesesForLet,
                         std::streampos &newLinePos) override;
};
