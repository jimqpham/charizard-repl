#include "num_expr.h"
#include "../Vals/num_val.h"

NumExpr::NumExpr(int val) {
    this->val = val;
}

bool NumExpr::equals(PTR(Expr) o) {
    PTR(NumExpr) numExpr = CAST(NumExpr)(o);
    if (numExpr == nullptr)
        return false;
    else
        return numExpr->val == this->val;
}

PTR(Val)NumExpr::interp_env(PTR(Env) env) {
    return NEW(NumVal)(val);
}

void NumExpr::print(std::ostream &out) {
    out << std::to_string(val);
}

void NumExpr::pretty_print_at(std::ostream &out,
                              precedence_t precedence,
                              bool kwrdNeedsPars,
                              std::streampos &newLinePos) {
    this->print(out);
}



