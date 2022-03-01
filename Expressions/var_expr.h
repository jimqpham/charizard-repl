#pragma once

#include <cstring>
#include "./expr.h"
#include <iostream>

class VarExpr : public Expr {
private:
    std::string name;

public:
    VarExpr(std::string name);

    bool equals(Expr *o) override;

    Val *interp() override;

    Expr *subst(std::string stringToMatch, Expr *replcExpr) override;

    void print(std::ostream &out) override;

    void pretty_print_at(std::ostream &out,
                         precedence_t precedence,
                         bool kwrdNeedsPars,
                         std::streampos &newLinePos) override;
};