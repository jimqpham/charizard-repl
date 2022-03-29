#include "expr.h"
#include "../shared_ptr.h"

class IfExpr : public Expr {
private:
    PTR(Expr) condition;
    PTR(Expr) thenBranch;
    PTR(Expr) elseBranch;

public:
    IfExpr(PTR(Expr) condition, PTR(Expr) thenBranch, PTR(Expr) elseBranch);

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
