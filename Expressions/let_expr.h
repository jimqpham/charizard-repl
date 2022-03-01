#include "expr.h"
#include "var_expr.h"

class Val;

class LetExpr : public Expr {
private:
    VarExpr *variable;
    Expr *rhs;
    Expr *body;

public:
    LetExpr(VarExpr *variable, Expr *rhs, Expr *body);

    bool equals(Expr *o) override;

    Val *interp() override;

    Expr *subst(std::string stringToMatch, Expr *replcExpr) override;

    void print(std::ostream &out) override;

    void pretty_print_at(
            std::ostream &out,
            precedence_t precedence,
            bool kwrdNeedsPars,
            std::streampos &newLinePos) override;
};
