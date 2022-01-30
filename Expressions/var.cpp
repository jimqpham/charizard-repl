#include "var.h"
#include <cstring>
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
    if (std::strcmp(this->name.c_str(), stringToMatch.c_str()) == 0)
        return replcExpr;

    return new Var(this->name);
}

void Var::print(std::ostream &out) {
    out << this->name;
}

void Var::pretty_print_at(std::ostream &out,
                          precedence_t precedence,
                          bool needsParenthesesForLet,
                          std::streampos &newLinePos) {
    this->print(out);
}
