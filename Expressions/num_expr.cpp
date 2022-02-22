#include "num_expr.h"
#include "../Vals/num_val.h"

NumExpr::NumExpr(int val) {
    this->val = val;
}

bool NumExpr::equals(Expr *o) {
    NumExpr *numExpr = dynamic_cast<NumExpr *>(o);
    if (numExpr == nullptr)
        return false;
    else
        return numExpr->val == this->val;
}

Val *NumExpr::interp() {
    return new NumVal(val);
}

bool NumExpr::has_variable() {
    return false;
}

Expr *NumExpr::subst(std::string stringToMatch, Expr *replcExpr) {
    return new NumExpr(this->val);
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



