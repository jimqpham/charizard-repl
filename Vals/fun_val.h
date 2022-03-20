#include <string>
#include "val.h"
#include "../shared_ptr.h"
#include "../Expressions/env.h"

class FunVal : public Val {
private:
    std::string formal_arg;
    PTR(Expr) body;
    PTR(Env) env;

public:
    FunVal(std::string, PTR(Expr), PTR(Env) = Env::empty);

    bool value_equals(PTR(Val) o) override;

    PTR(Val) add_to(PTR(Val) o) override;

    PTR(Val) mult_by(PTR(Val) o) override;

    std::string to_string() override;

    PTR(Val) call(PTR(Val) actual_arg) override;
};
