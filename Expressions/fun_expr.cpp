#include "fun_expr.h"
#include "../Vals/fun_val.h"

FunExpr::FunExpr(std::string formal_arg, PTR(Expr) body) {
    this->formal_arg = formal_arg;
    this->body = body;
}

bool FunExpr::equals(PTR(Expr) o) {
    PTR(FunExpr) otherFun = CAST(FunExpr)(o);

    return otherFun != nullptr &&
           otherFun->formal_arg == this->formal_arg &&
           otherFun->body->equals(this->body);
}

PTR(Val) FunExpr::interp_env(PTR(Env) env) {
    return NEW(FunVal)(this->formal_arg, this->body, env);
}

void FunExpr::print(std::ostream &out) {
    out << "(_fun (" << this->formal_arg << ") ";
    this->body->print(out);
    out << ")";
}

void FunExpr::pretty_print_at(std::ostream &out, precedence_t precedence,
                              bool kwrdNeedsPars, std::streampos &newLinePos) {
    std::streampos funCol = kwrdNeedsPars
                            ? out.tellp() - newLinePos + 1 // add one col for opening parenthesis
                            : out.tellp() - newLinePos;

    // PRINT LINE 1
    out << (kwrdNeedsPars ? "(" : "") << "_fun (" << this->formal_arg << ")\n";
    newLinePos = out.tellp();

    // PRINT LINE 2
    // Pad with whitespaces till 2 whitespaces past fun column
    for (int i = 0; i < (funCol + std::streampos(2)); i++) {
        out << " ";
    }
    body->pretty_print_at(out, prec_none, false, newLinePos);
    out << (kwrdNeedsPars ? ")" : "");
}
