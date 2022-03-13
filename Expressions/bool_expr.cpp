#include "../Vals/bool_val.h"

#include "bool_expr.h"

BoolExpr::BoolExpr(bool val) {
    this->val = val;
}

bool BoolExpr::equals(PTR(Expr)o) {
    PTR(BoolExpr)
    boolExpr = CAST(BoolExpr)(o);
    if (boolExpr == nullptr)
        return false;
    else
        return boolExpr->val == this->val;
}

PTR(Val)BoolExpr::interp() {
    return NEW(BoolVal)(val);
}

PTR(Expr)BoolExpr::subst(std::string stringToMatch, PTR(Expr)replcExpr) {
    return NEW(BoolExpr)(this->val);
}

void BoolExpr::print(std::ostream &out) {
    out << (val ? "_true" : "_false");
}

void BoolExpr::pretty_print_at(std::ostream &out, precedence_t precedence, bool kwrdNeedsPars,
                               std::streampos &newLinePos) {
    this->print(out);
}
