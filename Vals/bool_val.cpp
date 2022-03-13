#include "bool_val.h"
#include "../Expressions/bool_expr.h"

BoolVal::BoolVal(bool rep) {
    this->rep = rep;
}

bool BoolVal::value_equals(PTR(Val)o) {
    PTR(BoolVal)otherVal = dynamic_cast<BoolVal *>(o);

    if (otherVal != nullptr)
        return this->rep == otherVal->rep;
    else
        return false;
}

PTR(Val)BoolVal::add_to(PTR(Val)o) {
    throw std::runtime_error("add of non-number");
}

PTR(Val)BoolVal::mult_by(PTR(Val)o) {
    throw std::runtime_error("mult of non-number");
}

std::string BoolVal::to_string() {
    return this->rep ? "_true" : "_false";
}

PTR(Expr)BoolVal::to_expr() {
    return NEW(BoolExpr)(this->rep);
}

PTR(Val)BoolVal::call(PTR(Val)actual_arg) {
    throw std::runtime_error("call of non-function val");
}
