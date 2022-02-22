#include "var_expr.h"
#include "../Vals/val.h"
#include <cstring>
#include <stdexcept>

VarExpr::VarExpr(std::string name) {
    this->name = name;
}

bool VarExpr::equals(Expr *o) {
    VarExpr *varExpr = dynamic_cast<VarExpr *>(o);
    if (varExpr == nullptr)
        return false;
    else
        return (std::strcmp(this->name.c_str(), varExpr->name.c_str()) == 0);
}

Val *VarExpr::interp() {
    throw std::runtime_error("No value for variable");
}

bool VarExpr::has_variable() {
    return true;
}

Expr *VarExpr::subst(std::string stringToMatch, Expr *replcExpr) {
    if (std::strcmp(this->name.c_str(), stringToMatch.c_str()) == 0)
        return replcExpr;

    return new VarExpr(this->name);
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
