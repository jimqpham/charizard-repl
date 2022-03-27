#include "var_expr.h"
#include "../Vals/val.h"
#include <cstring>
#include <stdexcept>
#include "../Utils/env.h"

VarExpr::VarExpr(std::string name) {
    this->name = name;
}

bool VarExpr::equals(PTR(Expr) o) {
    PTR(VarExpr) varExpr = CAST(VarExpr)(o);
    if (varExpr == nullptr)
        return false;
    else
        return (std::strcmp(this->name.c_str(), varExpr->name.c_str()) == 0);
}

PTR(Val) VarExpr::interp_env(PTR(Env) env) {
    return env->lookup(this->name);
}

void VarExpr::print(std::ostream &out) {
    out << this->name;
}

void VarExpr::pretty_print_at(std::ostream &out,
                              precedence_t precedence,
                              bool kwrdNeedsPars,
                              std::streampos &newLinePos) {
    this->print(out);
}
