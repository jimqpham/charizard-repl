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

    PTR(Val) interp_env(PTR(Env) env) override;

    void step_interp() override;

    void print(std::ostream &out) override;

    void pretty_print_at(std::ostream &out,
                         precedence_t precedence,
                         bool kwrdNeedsPars,
                         std::streampos &newLinePos) override;
};