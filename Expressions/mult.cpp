#include "mult.h"

Mult::Mult(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool Mult::equals(Expr *o) {
    Mult *multExpr = dynamic_cast<Mult*>(o);
    if (multExpr == nullptr)
        return false;
    else
        return (multExpr->lhs->equals(this->lhs)) && (multExpr->rhs->equals(this->rhs));
}

int Mult::interp() {
    return (this->lhs->interp() * this->rhs->interp());
}

bool Mult::has_variable() {
    return (this->lhs->has_variable() || this->rhs->has_variable());
}

Expr *Mult::subst(std::string stringToMatch, Expr *replcExpr) {
    Expr* substLhs = this->lhs->subst(stringToMatch, replcExpr);
    Expr* substRhs = this->rhs->subst(stringToMatch, replcExpr);
    Mult* result = new Mult(substLhs, substRhs);
    return result;
}

void Mult::print(std::ostream &out) {
    out << std::string("(");
    this->lhs->print(out);
    out << std::string("*");
    this->rhs->print(out);
    out << std::string(")");
}

void Mult::pretty_print_at(std::ostream &out,
                           precedence_t precedence,
                           bool needsParenthesesForLet,
                           std::streampos &newLinePos) {
    if (precedence >= prec_mult) {
        out << std::string("(");
        this->lhs->pretty_print_at(out, prec_mult, true, newLinePos);
        out << std::string(" * ");
        this->rhs->pretty_print_at(out, prec_add, needsParenthesesForLet, newLinePos);
        out << std::string(")");
    }
    else {
        this->lhs->pretty_print_at(out, prec_mult, true, newLinePos);
        out << std::string(" * ");
        this->rhs->pretty_print_at(out, prec_add, needsParenthesesForLet, newLinePos);
    }
}
