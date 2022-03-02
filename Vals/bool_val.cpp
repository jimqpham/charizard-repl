#include "bool_val.h"
#include "../Expressions/bool_expr.h"

BoolVal::BoolVal(bool rep) {
    this->rep = rep;
}

bool BoolVal::value_equals(Val *o) {
    BoolVal *otherVal = dynamic_cast<BoolVal *>(o);

    if (otherVal != nullptr)
        return this->rep == otherVal->rep;
    else
        return false;
}

Val *BoolVal::add_to(Val *o) {
    throw std::runtime_error("add of non-number");
}

Val *BoolVal::mult_by(Val *o) {
    throw std::runtime_error("mult of non-number");
}

std::string BoolVal::to_string() {
    return this->rep ? "_true" : "_false";
}

Expr *BoolVal::to_expr() {
    return new BoolExpr(this->rep);
}

Val *BoolVal::call(Val *actual_arg) {
    return nullptr;
}
