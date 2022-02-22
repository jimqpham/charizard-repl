#include "expr.h"

class EqualExpr : public Expr {
private:
    Expr *lhs;
    Expr *rhs;

public:
    EqualExpr(Expr *lhs, Expr *rhs);

    bool equals(Expr *o) override;

    Val *interp() override;

    bool has_variable() override;

    Expr *subst(std::string stringToMatch, Expr *replcExpr) override;

    void print(std::ostream &out) override;

    void pretty_print_at(std::ostream &out,
                         precedence_t precedence,
                         bool kwrdNeedsPars,
                         std::streampos &newLinePos) override;
};
