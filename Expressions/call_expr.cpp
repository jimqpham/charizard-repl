#include "call_expr.h"
#include "../Vals/val.h"

CallExpr::CallExpr(Expr *to_be_called, Expr *actual_arg) {
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}

bool CallExpr::equals(Expr *o) {
    CallExpr *otherCallExpr = dynamic_cast<CallExpr *>(o);

    return otherCallExpr != nullptr &&
           otherCallExpr->to_be_called->equals(this->to_be_called) &&
           otherCallExpr->actual_arg->equals(this->actual_arg);
}

Val *CallExpr::interp() {
    return this->to_be_called->interp()->call(actual_arg->interp());
}

Expr *CallExpr::subst(std::string stringToMatch, Expr *replcExpr) {
    return new CallExpr(this->to_be_called->subst(stringToMatch, replcExpr),
                        this->actual_arg->subst(stringToMatch, replcExpr));
}

void CallExpr::print(std::ostream &out) {
    this->to_be_called->print(out);
    out << "(";
    this->actual_arg->print(out);
    out << ")";
}

void CallExpr::pretty_print_at(std::ostream &out, precedence_t precedence, bool kwrdNeedsPars,
                               std::streampos &newLinePos) {
    this->to_be_called->pretty_print_at(out, precedence, kwrdNeedsPars, newLinePos);
    out << "(";
    this->actual_arg->pretty_print_at(out, prec_none, false, newLinePos);
    out << ")";
}
