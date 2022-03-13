#include <string>
#include "val.h"
#include "../shared_ptr.h"

class FunVal : public Val {
private:
    std::string formal_arg;
    Expr *body;

public:
    FunVal(std::string, Expr *);

    bool value_equals(Val *o) override;

    Val *add_to(Val *o) override;

    Val *mult_by(Val *o) override;

    std::string to_string() override;

    Expr *to_expr() override;

    Val *call(Val *actual_arg) override;
};
