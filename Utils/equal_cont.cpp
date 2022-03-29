#include "equal_cont.h"
#include "step.h"
#include "../Vals/val.h"
#include "../Vals/bool_val.h"

void RightThenCompCont::step_continue() {
    PTR(Val) lhs_val = Step::val;
    Step::mode = Step::interp_mode;
    Step::expr = rhs;
    Step::env = env;
    Step::cont = NEW(CompCont)(lhs_val, rest);
}

void CompCont::step_continue() {
    PTR(Val) rhs_val = Step::val;
    Step::mode = Step::continue_mode;
    Step::val = NEW(BoolVal)(lhs_val->value_equals(rhs_val));
    Step::cont = rest;
}
