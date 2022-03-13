#pragma once

#include <cstring>
#include "./expr.h"
#include "../shared_ptr.h"
#include <iostream>

class VarExpr : public Expr {
private:
    std::string name;

public:
    VarExpr(std::string name);

    bool equals(PTR(Expr) o) override;

    PTR(Val) interp() override;

    PTR(Expr) subst(std::string stringToMatch, PTR(Expr) replcExpr) override;

    void print(std::ostream &out) override;

    void pretty_print_at(std::ostream &out,
                         precedence_t precedence,
                         bool kwrdNeedsPars,
                         std::streampos &newLinePos) override;
};