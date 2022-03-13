#pragma once

#include "expr.h"
#include <iostream>
#include "../shared_ptr.h"

class Val;

class AddExpr : public Expr {
private:
    PTR(Expr) lhs;
    PTR(Expr) rhs;

public:
    AddExpr(PTR(Expr) lhs, PTR(Expr) rhs);

    bool equals(PTR(Expr) o) override;

    PTR(Val) interp() override;

    PTR(Expr) subst(std::string stringToMatch, PTR(Expr) replcExpr) override;

    void print(std::ostream &out) override;

    void pretty_print_at(std::ostream &out,
                         precedence_t precedence,
                         bool kwrdNeedsPars,
                         std::streampos &newLinePos) override;
};
