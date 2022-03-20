#include <string>
#include "val.h"
#include "../shared_ptr.h"

class NumVal : public Val {
private:
    int rep;

public:
    explicit NumVal(int rep);

    bool value_equals(PTR(Val) o) override;

    PTR(Val) add_to(PTR(Val) o) override;

    PTR(Val) mult_by(PTR(Val) o) override;

    std::string to_string() override;

    PTR(Val) call(PTR(Val) actual_arg) override;
};
