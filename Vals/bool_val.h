#include <string>
#include "val.h"
#include "../shared_ptr.h"

class BoolVal : public Val {
private:
    bool rep;

public:
    explicit BoolVal(bool rep);

    bool value_equals(PTR(Val) o) override;

    PTR(Val) add_to(PTR(Val) o) override;

    PTR(Val) mult_by(PTR(Val) o) override;

    std::string to_string() override;

    PTR(Val) call(PTR(Val) actual_arg) override;

    void call_step(PTR(Val) actual_arg_val, PTR(Cont) rest) override;
};
