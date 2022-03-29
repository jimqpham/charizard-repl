#include "expr.h"
#include "../shared_ptr.h"

class CallExpr : public Expr {
private:
    PTR(Expr) to_be_called;
    PTR(Expr) actual_arg;

public:
    CallExpr(PTR(Expr) to_be_called, PTR(Expr) actual_arg);

    bool equals(PTR(Expr) o) override;

    PTR(Val) interp_env(PTR(Env) env) override;

    void step_interp() override;

    void print(std::ostream &out) override;

    void pretty_print_at(std::ostream &out,
                         precedence_t precedence,
                         bool kwrdNeedsPars,
                         std::streampos &newLinePos) override;
};
