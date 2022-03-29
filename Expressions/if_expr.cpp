#include "if_expr.h"
#include "../Vals/bool_val.h"
#include "../Utils/env.h"
#include "../Utils/step.h"
#include "../Utils/if_cont.h"


IfExpr::IfExpr(PTR(Expr) condition, PTR(Expr) thenBranch, PTR(Expr) elseBranch) {
    this->condition = condition;
    this->thenBranch = thenBranch;
    this->elseBranch = elseBranch;
}

bool IfExpr::equals(PTR(Expr) o) {
    PTR(IfExpr) otherIf = CAST(IfExpr)(o);
    if (otherIf != nullptr)
        return otherIf->condition->equals(this->condition) &&
               otherIf->thenBranch->equals(this->thenBranch) &&
               otherIf->elseBranch->equals(this->elseBranch);
    else
        return false;
}

PTR(Val) IfExpr::interp_env(PTR(Env) env) {
    PTR(Val) conditionVal = this->condition->interp_env(env);
    if (conditionVal->value_equals(NEW(BoolVal)(true)))
        return this->thenBranch->interp_env(env);
    else if (conditionVal->value_equals(NEW(BoolVal)(false)))
        return this->elseBranch->interp_env(env);
    else
        throw std::runtime_error("non-boolean condition in if body");
}

void IfExpr::step_interp() {
    Step::mode = Step::interp_mode;
    Step::expr = condition;
    Step::env = Step::env;
    Step::cont = NEW(IfBranchCont)(thenBranch, elseBranch, Step::env, Step::cont);
}

void IfExpr::print(std::ostream &out) {
    out << "(_if ";
    condition->print(out);
    out << " _then ";
    thenBranch->print(out);
    out << " _else ";
    elseBranch->print(out);
    out << ")";
}

void IfExpr::pretty_print_at(std::ostream &out, precedence_t precedence, bool kwrdNeedsPars,
                             std::streampos &newLinePos) {
    std::streampos ifCol = kwrdNeedsPars
                           ? out.tellp() - newLinePos + 1 // add one more column for the opening parenthesis
                           : out.tellp() - newLinePos;
    std::string openingBracket = kwrdNeedsPars ? "(" : "";
    std::string closingBracket = kwrdNeedsPars ? ")" : "";

    // PRINT LINE 1
    out << openingBracket << "_if ";
    condition->pretty_print_at(out, prec_none, false, newLinePos);

    // PRINT LINE 2
    out << "\n";
    newLinePos = out.tellp();
    // Pad with whitespaces till ifCol
    for (int i = 0; i < ifCol; i++) {
        out << " ";
    }
    // Line content
    out << "_then ";
    thenBranch->pretty_print_at(out, prec_none, false, newLinePos);

    // PRINT LINE 3
    out << "\n";
    newLinePos = out.tellp();
    // Pad with whitespaces till ifCol
    for (int i = 0; i < ifCol; i++) {
        out << " ";
    }
    // Line content
    out << "_else ";
    elseBranch->pretty_print_at(out, prec_none, false, newLinePos);

    out << closingBracket;
}
