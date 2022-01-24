#include "var.h"
#include "string"
#include <stdexcept>

Var::Var(std::string name) {
    this->name = name;
}

bool Var::equals(Expr *o) {
    Var* varExpr = dynamic_cast<Var*>(o);
    if (varExpr == nullptr)
        return false;
    else
        return (std::strcmp(this->name.c_str(), varExpr->name.c_str()) == 0);
}

int Var::interp() {
    throw std::runtime_error("No value for variable");
}

bool Var::has_variable() {
    return true;
}

Expr *Var::subst(std::string stringToMatch, Expr *replcExpr) {
    if (strcmp(this->name.c_str(), stringToMatch.c_str()) == 0)
        return replcExpr;

    return new Var(this->name);
}