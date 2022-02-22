#include "expr.h"

class IfExpr : public Expr {
private:
    Expr *condition;
    Expr *thenBranch;
    Expr *elseBranch;

public:
    IfExpr(Expr *condition, Expr *thenBranch, Expr *elseBranch);

    bool equals(Expr *o) override;

    Val *interp() override;

    bool has_variable() override;

    Expr *subst(std::string stringToMatch, Expr *replcExpr) override;

    void print(std::ostream &out) override;

    void pretty_print_at(
            std::ostream &out,
            precedence_t precedence,
            bool kwrdNeedsPars,
            std::streampos &newLinePos) override;
};
