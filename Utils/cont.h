#pragma once

#include "../shared_ptr.h"

CLASS(Cont) {
public:
    static PTR(Cont) done;

    virtual void step_continue() = 0;
};

class DoneCont : public Cont {
    void step_continue() override;
};