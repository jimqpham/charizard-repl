#include "num_val.h"
#include "../Expressions/num_expr.h"

NumVal::NumVal(int rep) {
    this->rep = rep;
}

bool NumVal::value_equals(Val *o) {
    NumVal *otherVal = dynamic_cast<NumVal *>(o);

    if (otherVal != nullptr)
        return this->rep == otherVal->rep;
    else
        return false;
}

Val *NumVal::add_to(Val *o) {
    NumVal *otherVal = dynamic_cast<NumVal *>(o);

    if (otherVal != nullptr)
        return new NumVal(this->rep + otherVal->rep);
    else
        throw std::runtime_error("add of non-number");
}

Val *NumVal::mult_by(Val *o) {
    NumVal *otherVal = dynamic_cast<NumVal *>(o);

    if (otherVal != nullptr)
        return new NumVal(this->rep * otherVal->rep);
    else
        throw std::runtime_error("mult of non-number");
}

std::string NumVal::to_string() {
    return std::to_string(this->rep);
}

Expr *NumVal::to_expr() {
    return new NumExpr(this->rep);
}

Val *NumVal::call(Val *actual_arg) {
    return new NumVal(this->rep);
}
