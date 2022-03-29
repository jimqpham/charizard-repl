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

    PTR(Val) interp_env(PTR(Env) env) override;

    void step_interp() override;

    void print(std::ostream &out) override;

    void pretty_print_at(
            std::ostream &out,
            precedence_t precedence,
            bool kwrdNeedsPars,
            std::streampos &newLinePos) override;
};
