#include "num.h"

Num::Num(int val) {
    this->val = val;
}

bool Num::equals(Expr *o) {
    Num *numExpr = dynamic_cast<Num*>(o);
    if (numExpr == nullptr)
        return false;
    else
        return numExpr->val == this->val;
}

int Num::interp() {
    return val;
}

bool Num::has_variable() {
    return false;
}

Expr *Num::subst(std::string stringToMatch, Expr *replcExpr) {
    return new Num(this->val);
}

void Num::print(std::ostream &out) {
    out << std::to_string(val);
}

void Num::pretty_print_at(std::ostream &out, precedence_t precedence, bool needsParenthesesForLet) {
    this->print(out);
}



