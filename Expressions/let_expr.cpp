#include "let_expr.h"
#include "num_expr.h"
#include "../Vals/val.h"
#include "../Utils/extended_env.h"
#include "../Utils/step.h"
#include "../Utils/cont.h"

LetExpr::LetExpr(PTR(VarExpr) variable, PTR(Expr) rhs, PTR(Expr) body) {
    this->variable = variable;
    this->rhs = rhs;
    this->body = body;
}

bool LetExpr::equals(PTR(Expr) o) {
    PTR(LetExpr) letExpr = CAST(LetExpr)(o);
    if (letExpr == nullptr)
        return false;
    return (this->variable->equals(letExpr->variable) &&
            this->rhs->equals(letExpr->rhs) &&
            this->body->equals(letExpr->body));
}

PTR(Val) LetExpr::interp_env(PTR(Env) env) {
    PTR(Val) evalRhs = this->rhs->interp_env(env);
    PTR(Env) new_env = NEW(ExtendedEnv)(variable->to_string(), evalRhs, env);
    return body->interp_env(new_env);
}

void LetExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = rhs;
    Step::env = Step::env;
    Step::cont = NEW(LetBodyCont)(variable->to_string(), body, Step::env, Step::cont);

}

void LetExpr::print(std::ostream &out) {
    out << "(_let ";
    variable->print(out);
    out << "=";
    rhs->print(out);
    out << " _in ";
    body->print(out);
    out << ")";
}

void LetExpr::pretty_print_at(std::ostream &out,
                              precedence_t precedence,
                              bool kwrdNeedsPars,
                              std::streampos &newLinePos) {

    std::streampos letColumn = kwrdNeedsPars
                               ? out.tellp() - newLinePos + 1 // add one more column for the opening parenthesis
                               : out.tellp() - newLinePos;
    std::string openingBracket = kwrdNeedsPars ? "(" : "";
    std::string closingBracket = kwrdNeedsPars ? ")" : "";

    // PRINT LINE 1
    out << openingBracket << "_let ";
    variable->print(out);
    out << " = ";
    rhs->pretty_print_at(out, prec_none, false, newLinePos);

    // PRINT LINE 2
    out << "\n";
    newLinePos = out.tellp();
    // Pad with whitespaces till letColumn
    for (int i = 0; i < letColumn; i++) {
        out << " ";
    }
    // Start content of line 2
    out << "_in  ";
    body->pretty_print_at(out, prec_none, false, newLinePos);
    out << closingBracket;
}
