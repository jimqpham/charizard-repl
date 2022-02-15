//
// Created by Quang Pham on 1/28/22.
//

#include "let.h"
#include "num.h"

Let::Let(Var *variable, Expr *rhs, Expr *body) {
    this->variable = variable;
    this->rhs = rhs;
    this->body = body;
}

bool Let::equals(Expr *o) {
    Let *letExpr = dynamic_cast<Let *>(o);
    if (letExpr == nullptr)
        return false;
    return (this->variable->equals(letExpr->variable) &&
            this->rhs->equals(letExpr->rhs) &&
            this->body->equals(letExpr->body));
}

int Let::interp() {
    int evalRhs = this->rhs->interp();
    Expr *substBody = this->body->subst(this->variable->name, new Num(evalRhs));
    return substBody->interp();
}

bool Let::has_variable() {
    return (this->rhs->has_variable() || this->body->has_variable());
}

Expr *Let::subst(std::string stringToMatch, Expr *replcExpr) {
    Expr *substRhs = this->rhs->subst(stringToMatch, replcExpr);

    // If the variable bound by let does not match the variable to replace (stringToMatch)
    // go into the body
    if (!variable->equals(new Var(stringToMatch))) {
        Expr *substBody = this->body->subst(stringToMatch, replcExpr);
        return new Let(this->variable, substRhs, substBody);
    }

    return new Let(this->variable, substRhs, body);
}

void Let::print(std::ostream &out) {
    out << "(_let ";
    variable->print(out);
    out << "=";
    rhs->print(out);
    out << " _in ";
    body->print(out);
    out << ")";
}

void Let::pretty_print_at(std::ostream &out,
                          precedence_t precedence,
                          bool needsParenthesesForLet,
                          std::streampos &newLinePos) {

    std::streampos letColumn = needsParenthesesForLet
                               ? out.tellp() - newLinePos + 1 // add one more column for the opening parenthesis
                               : out.tellp() - newLinePos;
    std::string openingBracket = needsParenthesesForLet ? "(" : "";
    std::string closingBracket = needsParenthesesForLet ? ")" : "";

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
