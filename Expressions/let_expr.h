#include "expr.h"
#include "var_expr.h"
#include "../shared_ptr.h"

class Val;

class LetExpr : public Expr {
private:
    PTR(VarExpr) variable;
    PTR(Expr) rhs;
    PTR(Expr) body;

public:
    LetExpr(PTR(VarExpr) variable, PTR(Expr) rhs, PTR(Expr) body);

    bool equals(PTR(Expr) o) override;

    PTR(Val) interp() override;

    PTR(Expr) subst(std::string stringToMatch, PTR(Expr) replcExpr) override;

    void print(std::ostream &out) override;

    void pretty_print_at(
            std::ostream &out,
            precedence_t precedence,
            bool kwrdNeedsPars,
            std::streampos &newLinePos) override;
};
