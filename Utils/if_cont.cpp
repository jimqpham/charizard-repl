#include "if_cont.h"
#include "step.h"
#include "../Vals/val.h"
#include "../Vals/bool_val.h"

void IfBranchCont::step_continue() {
    PTR(Val) test_val = Step::val;
    Step::mode = Step::interp_mode;
    if (test_val->value_equals(NEW(BoolVal)(true)))
        Step::expr = then_part;
    else
        Step::expr = else_part;
    Step::env = env;
    Step::cont = rest;
}
