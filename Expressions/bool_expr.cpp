#include "../Vals/bool_val.h"

#include "bool_expr.h"

BoolExpr::BoolExpr(bool val) {
    this->val = val;
}

bool BoolExpr::equals(Expr *o) {
    BoolExpr *boolExpr = dynamic_cast<BoolExpr *>(o);
    if (boolExpr == nullptr)
        return false;
    else
        return boolExpr->val == this->val;
}

Val *BoolExpr::interp() {
    return new BoolVal(val);
}

Expr *BoolExpr::subst(std::string stringToMatch, Expr *replcExpr) {
    return new BoolExpr(this->val);
}

void BoolExpr::print(std::ostream &out) {
    out << (val ? "_true" : "_false");
}

void BoolExpr::pretty_print_at(std::ostream &out, precedence_t precedence, bool kwrdNeedsPars,
                               std::streampos &newLinePos) {
    this->print(out);
}
