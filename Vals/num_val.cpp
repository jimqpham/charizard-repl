#include "num_val.h"
#include "../Expressions/num_expr.h"

NumVal::NumVal(int rep) {
    this->rep = rep;
}

bool NumVal::value_equals(PTR(Val) o) {
    PTR(NumVal) otherVal = dynamic_cast<NumVal *>(o);

    if (otherVal != nullptr)
        return this->rep == otherVal->rep;
    else
        return false;
}

PTR(Val)NumVal::add_to(PTR(Val) o) {
    PTR(NumVal) otherVal = dynamic_cast<NumVal *>(o);

    if (otherVal != nullptr)
        return new NumVal(this->rep + otherVal->rep);
    else
        throw std::runtime_error("add of non-number");
}

PTR(Val)NumVal::mult_by(PTR(Val) o) {
    PTR(NumVal) otherVal = dynamic_cast<NumVal *>(o);

    if (otherVal != nullptr)
        return new NumVal(this->rep * otherVal->rep);
    else
        throw std::runtime_error("mult of non-number");
}

std::string NumVal::to_string() {
    return std::to_string(this->rep);
}

PTR(Expr)NumVal::to_expr() {
    return new NumExpr(this->rep);
}

PTR(Val)NumVal::call(PTR(Val) actual_arg) {
    throw std::runtime_error("call of non-function val");
}
