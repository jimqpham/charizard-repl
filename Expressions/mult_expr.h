#pragma once

#include "expr.h"
#include "../shared_ptr.h"
#include <iostream>

class MultExpr : public Expr {
private:
    PTR(Expr) lhs;
    PTR(Expr) rhs;

public:
    MultExpr(PTR(Expr) lhs, PTR(Expr) rhs);

    bool equals(PTR(Expr) o) override;

    PTR(Val) interp_env(PTR(Env) env) override;

    void step_interp() override;

    void print(std::ostream &out) override;

    void pretty_print_at(std::ostream &out,
                         precedence_t precedence,
                         bool kwrdNeedsPars,
                         std::streampos &newLinePos) override;
};