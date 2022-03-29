#include "cont.h"

PTR(Cont) Cont::done = NEW(DoneCont)();

void DoneCont::step_continue() {
    throw std::runtime_error("can't continue done");
}
