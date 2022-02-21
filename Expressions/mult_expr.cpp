#include "mult_expr.h"
#include "../Vals/val.h"

MultExpr::MultExpr(Expr *lhs, Expr *rhs) {
    this->lhs = lhs;
    this->rhs = rhs;
}

bool MultExpr::equals(Expr *o) {
    MultExpr *multExpr = dynamic_cast<MultExpr *>(o);
    if (multExpr == nullptr)
        return false;
    else
        return (multExpr->lhs->equals(this->lhs)) && (multExpr->rhs->equals(this->rhs));
}

Val *MultExpr::interp() {
    return (this->lhs->interp()->mult_by(this->rhs->interp()));
}

bool MultExpr::has_variable() {
    return (this->lhs->has_variable() || this->rhs->has_variable());
}

Expr *MultExpr::subst(std::string stringToMatch, Expr *replcExpr) {
    Expr *substLhs = this->lhs->subst(stringToMatch, replcExpr);
    Expr *substRhs = this->rhs->subst(stringToMatch, replcExpr);
    MultExpr *result = new MultExpr(substLhs, substRhs);
    return result;
}

void MultExpr::print(std::ostream &out) {
    out << std::string("(");
    this->lhs->print(out);
    out << std::string("*");
    this->rhs->print(out);
    out << std::string(")");
}

void MultExpr::pretty_print_at(std::ostream &out,
                               precedence_t precedence,
                               bool needsParenthesesForLet,
                               std::streampos &newLinePos) {
    if (precedence >= prec_mult) {
        out << std::string("(");
        this->lhs->pretty_print_at(out, prec_mult, true, newLinePos);
        out << std::string(" * ");
        this->rhs->pretty_print_at(out, prec_add, needsParenthesesForLet, newLinePos);
        out << std::string(")");
    } else {
        this->lhs->pretty_print_at(out, prec_mult, true, newLinePos);
        out << std::string(" * ");
        this->rhs->pretty_print_at(out, prec_add, needsParenthesesForLet, newLinePos);
    }
}
