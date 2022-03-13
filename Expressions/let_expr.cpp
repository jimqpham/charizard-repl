//
// Created by Quang Pham on 1/28/22.
//

#include "let_expr.h"
#include "num_expr.h"
#include "../Vals/val.h"

LetExpr::LetExpr(PTR(VarExpr)variable, PTR(Expr)rhs, PTR(Expr)body) {
    this->variable = variable;
    this->rhs = rhs;
    this->body = body;
}

bool LetExpr::equals(PTR(Expr)o) {
    PTR(LetExpr)letExpr = dynamic_cast<PTR(LetExpr) >(o);
    if (letExpr == nullptr)
        return false;
    return (this->variable->equals(letExpr->variable) &&
            this->rhs->equals(letExpr->rhs) &&
            this->body->equals(letExpr->body));
}

PTR(Val)LetExpr::interp() {
    PTR(Val)evalRhs = this->rhs->interp();
    PTR(Expr)substBody = this->body->subst(this->variable->to_string(), evalRhs->to_expr());
    return substBody->interp();
}

PTR(Expr)LetExpr::subst(std::string stringToMatch, PTR(Expr)replcExpr) {
    PTR(Expr)substRhs = this->rhs->subst(stringToMatch, replcExpr);

    // If the arg bound by let does not match the arg to replace (stringToMatch)
    // go into the body
    if (!variable->equals(NEW(VarExpr)(stringToMatch))) {
        PTR(Expr)substBody = this->body->subst(stringToMatch, replcExpr);
        return NEW(LetExpr)(this->variable, substRhs, substBody);
    }

    return NEW(LetExpr)(this->variable, substRhs, body);
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
