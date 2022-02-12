#include <sstream>
#include "parse.h"
#include "num.h"
#include "var.h"
#include "mult.h"
#include "add.h"
#include "let.h"


Expr *parse_num(std::istream &in) {
    int n = 0;
    bool negative = false;

    if (in.peek() == '-') {
        negative = true;
        consume(in, '-');
    }

    if (!isdigit(in.peek()))
        throw std::runtime_error("Invalid minus sign placement");

    while (true) {
        int c = in.peek();
        if (isdigit(c)) {
            consume(in, c);
            n = n * 10 + (c - '0');
        } else
            break;
    }

    if (negative)
        n = -n;

    return new Num(n);
}

Var *parse_var(std::istream &in) {
    std::string name = "";

    while (true) {
        int c = in.peek();
        if (isalpha(c)) {
            consume(in, c);
            name += ((char) c);
        } else
            break;
    }

    return new Var(name);
}

Expr *parse_let(std::istream &in) {
    parse_keyword(in, "_let");

    if (!isspace(in.peek()))
        throw std::runtime_error("Whitespace error after _let");

    skip_whitespace(in);

    Var *variable = parse_var(in);

    skip_whitespace(in);

    check_and_consume(in, '=');

    skip_whitespace(in);

    Expr *rhs = parse_expr(in);

    skip_whitespace(in);

    parse_keyword(in, "_in");

    if (!isspace(in.peek()))
        throw std::runtime_error("Whitespace error after _in");

    skip_whitespace(in);

    Expr *body = parse_expr(in);

    return new Let(variable, rhs, body);
}

Expr *parse_multicand(std::istream &in) {
    skip_whitespace(in);

    int c = in.peek();
    if ((c == '-') || isdigit(c))
        return parse_num(in);
    else if (c == '(') {
        consume(in, '(');
        Expr *e = parse_expr(in);
        skip_whitespace(in);
        c = in.get();
        if (c != ')')
            throw std::runtime_error("Missing close parenthesis");
        return e;
    } else if (isalpha(c)) {
        return parse_var(in);
    } else if (c == '_') {
        return parse_let(in);
    } else {
        consume(in, c);
        throw std::runtime_error("Invalid input!");
    }
}

Expr *parse_addend(std::istream &in) {
    skip_whitespace(in);

    Expr *e;

    e = parse_multicand(in);

    skip_whitespace(in);

    int c = in.peek();
    if (c == '*') {
        consume(in, '*');
        Expr *rhs = parse_addend(in);
        return new Mult(e, rhs);
    } else
        return e;
}

Expr *parse_expr(std::istream &in) {
    skip_whitespace(in);

    Expr *e = parse_addend(in);

    skip_whitespace(in);

    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        Expr *rhs = parse_expr(in);
        return new Add(e, rhs);
    } else
        return e;
}

Expr *parse(std::istream &in) {
    skip_whitespace(in);

    Expr *e = parse_expr(in);

    skip_whitespace(in);

    if (!in.eof())
        throw std::runtime_error("Unexpected input after expression!");

    return e;
}

Expr *parse_str(std::string s) {
    std::istringstream in(s);

    return parse(in);
}

void check_and_consume(std::istream &in, char expected) {

    std::string message = "Missing character: Expect ";
    message += expected;

    if (in.peek() != expected)
        throw std::runtime_error(message);
    else
        consume(in, expected);
}

void parse_keyword(std::istream &in, std::string expected) {
    for (std::string::iterator iterator = expected.begin();
         iterator < expected.end(); iterator++) {
        check_and_consume(in, *iterator);
    }
}

void consume(std::istream &in, int expect) {
    int c = in.get();
    assert (c != expect)
}

void skip_whitespace(std::istream &in) {
    while (true) {
        int c = in.peek();
        if (!isspace(c))
            break;
        consume(in, c);
    }
}