#include "call_cont.h"
#include "step.h"
#include "../Vals/fun_val.h"

void CallCont::step_continue() {
    to_be_called_val->call_step(Step::val, rest);
}

void ArgThenCallCont::step_continue() {
    Step::mode = Step::interp_mode;
    Step::expr = actual_arg;
    Step::env = env;
    Step::cont = NEW(CallCont)(Step::val, rest);
}