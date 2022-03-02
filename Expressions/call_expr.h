#include "expr.h"

class CallExpr : public Expr {
private:
    Expr *to_be_called;
    Expr *actual_arg;

public:
    CallExpr(Expr *to_be_called, Expr *actual_arg);

    bool equals(Expr *o) override;

    Val *interp() override;

    Expr *subst(std::string stringToMatch, Expr *replcExpr) override;

    void print(std::ostream &out) override;

    void pretty_print_at(std::ostream &out,
                         precedence_t precedence,
                         bool kwrdNeedsPars,
                         std::streampos &newLinePos) override;
};
