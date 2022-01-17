#include "var.h"
#include "string"

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

