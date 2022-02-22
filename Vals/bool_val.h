#include <string>
#include "val.h"

class BoolVal : public Val {
private:
    bool rep;

public:
    explicit BoolVal(bool rep);

    bool value_equals(Val *o) override;

    Val *add_to(Val *o) override;

    Val *mult_by(Val *o) override;

    std::string to_string() override;

    Expr *to_expr() override;
};
