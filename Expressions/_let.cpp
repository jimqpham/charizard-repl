//
// Created by Quang Pham on 1/28/22.
//

#include "_let.h"
#include "num.h"

Let::Let(Var* variable, Expr *rhs, Expr *body) {
    this->variable = variable;
    this->rhs = rhs;
    this->body = body;
}

bool Let::equals(Expr *o) {
    Let* letExpr = dynamic_cast<Let*>(o);
    if (letExpr == nullptr)
        return false;
    return (this->variable->equals(letExpr->variable) &&
            this->rhs->equals(letExpr->rhs) &&
            this->body->equals(letExpr->body));
}

int Let::interp() {
    // Bottom-up approach: Evaluate the body before the outer let expression. This is because if the body is a
    // let expression (the inner let) and the same variable is used in the rhs's of both let expressions,
    // it will go with the inner rhs. This will throw an error if one or more variables
    // are free in the body (no bindings found).
    try {
        return this->body->interp();
    }
    // If the body is not self-sufficient (i.e. has free variables), go up one level to match the outer bindings
    // with the free variables
    catch (std::runtime_error &re) {
        int evalRhs = this->rhs->interp();
        Expr *substBody = this->body->subst(this->variable->name, new Num(evalRhs));
        return substBody->interp();
    }
}

bool Let::has_variable() {
    return (this->rhs->has_variable() || this->body->has_variable());
}

Expr *Let::subst(std::string stringToMatch, Expr *replcExpr) {
    Expr* substRhs = this->rhs->subst(stringToMatch, replcExpr);
    Expr* substBody = this->body->subst(stringToMatch, replcExpr);
    return new Let(this->variable, substRhs, substBody);
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

void Let::pretty_print_at(std::ostream &out, precedence_t precedence, bool needsParenthesesForLet) {
    if (needsParenthesesForLet) {
        out << "(_let ";
        variable->pretty_print(out);
        out << " = ";
        rhs->pretty_print(out);
        out << "\n _in  ";
        body->pretty_print(out);
        out << ")";
    }
    else {
        out << "_let ";
        variable->pretty_print(out);
        out << " = ";
        rhs->pretty_print(out);
        out << "\n_in  ";
        body->pretty_print(out);
    }
}
