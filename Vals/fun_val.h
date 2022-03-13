#include <string>
#include "val.h"
#include "../shared_ptr.h"

class FunVal : public Val {
private:
    std::string formal_arg;
    PTR(Expr) body;

public:
    FunVal(std::string, Expr *);

    bool value_equals(PTR(Val) o) override;

    PTR(Val) add_to(PTR(Val) o) override;

    PTR(Val) mult_by(PTR(Val) o) override;

    std::string to_string() override;

    PTR(Expr) to_expr() override;

    PTR(Val) call(PTR(Val)actual_arg) override;
};
