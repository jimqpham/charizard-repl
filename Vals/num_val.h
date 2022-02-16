#include <string>
#include "val.h"

class NumVal : public Val {
private:
    int rep;

public:
    explicit NumVal(int rep);

    bool equals(Val *o) override;

    Val *add_to(Val *o) override;

    Val *times(Val *o) override;

    std::string to_string() override;

    Expr *to_expr() override;
};
