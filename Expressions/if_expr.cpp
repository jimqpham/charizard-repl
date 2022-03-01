#include "if_expr.h"
#include "../Vals/bool_val.h"

IfExpr::IfExpr(Expr *condition, Expr *thenBranch, Expr *elseBranch) {
    this->condition = condition;
    this->thenBranch = thenBranch;
    this->elseBranch = elseBranch;
}

bool IfExpr::equals(Expr *o) {
    IfExpr *otherIf = dynamic_cast<IfExpr *>(o);
    if (otherIf != nullptr)
        return otherIf->condition->equals(this->condition) &&
               otherIf->thenBranch->equals(this->thenBranch) &&
               otherIf->elseBranch->equals(this->elseBranch);
    else
        return false;
}

Val *IfExpr::interp() {
    Val *conditionVal = this->condition->interp();
    if (conditionVal->value_equals(new BoolVal(true)))
        return this->thenBranch->interp();
    else if (conditionVal->value_equals(new BoolVal(false)))
        return this->elseBranch->interp();
    else
        throw std::runtime_error("non-boolean condition in if expr");
}

Expr *IfExpr::subst(std::string stringToMatch, Expr *replcExpr) {
    return new IfExpr(condition->subst(stringToMatch, replcExpr),
                      thenBranch->subst(stringToMatch, replcExpr),
                      elseBranch->subst(stringToMatch, replcExpr));
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
