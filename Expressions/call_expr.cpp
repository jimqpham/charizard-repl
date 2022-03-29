#include "call_expr.h"
#include "../Vals/val.h"
#include "../Utils/env.h"
#include "../Utils/step.h"
#include "../Utils/cont.h"

CallExpr::CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg) {
    this->to_be_called = to_be_called;
    this->actual_arg = actual_arg;
}

bool CallExpr::equals(PTR(Expr) o) {
    PTR(CallExpr) otherCallExpr = CAST(CallExpr)(o);

    return otherCallExpr != nullptr &&
           otherCallExpr->to_be_called->equals(this->to_be_called) &&
           otherCallExpr->actual_arg->equals(this->actual_arg);
}

PTR(Val) CallExpr::interp_env(PTR(Env) env) {
    return this->to_be_called->interp_env(env)->call(actual_arg->interp_env(env));
}

void CallExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = to_be_called;
    Step::cont = NEW(ArgThenCallCont)(actual_arg, Step::env, Step::cont);
}

void CallExpr::print(std::ostream &out) {
    this->to_be_called->print(out);
    out << "(";
    this->actual_arg->print(out);
    out << ")";
}

void CallExpr::pretty_print_at(std::ostream &out, precedence_t precedence, bool kwrdNeedsPars,
                               std::streampos &newLinePos) {
    // Add parentheses if to_be_called is a keyword expr/binary-op expr
    this->to_be_called->pretty_print_at(out, prec_mult, true, newLinePos);
    out << "(";
    this->actual_arg->pretty_print_at(out, prec_none, false, newLinePos);
    out << ")";
}
