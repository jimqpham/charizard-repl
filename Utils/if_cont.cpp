#include "if_cont.h"
#include "step.h"
#include "../Vals/val.h"
#include "../Vals/bool_val.h"

void IfBranchCont::step_continue() {
    PTR(Val) test_val = Step::val;
    Step::mode = Step::interp_mode;
    if (test_val->value_equals(NEW(BoolVal)(true)))
        Step::expr = then_part;
    else if (test_val->value_equals(NEW(BoolVal)(false)))
        Step::expr = else_part;
    else
        throw std::runtime_error("non-boolean condition in if body");
    Step::env = env;
    Step::cont = rest;
}
