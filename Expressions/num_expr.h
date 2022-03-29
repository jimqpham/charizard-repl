#pragma once

#include "expr.h"
#include <iostream>
#include "../shared_ptr.h"

class NumExpr : public Expr {
private:
    int val;

public:
    NumExpr(int val);

    bool equals(PTR(Expr) o) override;

    PTR(Val) interp_env(PTR(Env) env) override;

    void step_interp() override;

    void print(std::ostream &out) override;

    void pretty_print_at(std::ostream &out,
                         precedence_t precedence,
                         bool kwrdNeedsPars,
                         std::streampos &newLinePos) override;
};


