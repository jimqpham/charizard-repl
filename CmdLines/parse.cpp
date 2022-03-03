#include <sstream>
#include "parse.h"
#include "../Expressions/num_expr.h"
#include "../Expressions/var_expr.h"
#include "../Expressions/mult_expr.h"
#include "../Expressions/add_expr.h"
#include "../Expressions/let_expr.h"
#include "../Expressions/if_expr.h"
#include "../Expressions/bool_expr.h"
#include "../Expressions/equal_expr.h"
#include "../Expressions/call_expr.h"
#include "../Expressions/fun_expr.h"


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

    return new NumExpr(n);
}

VarExpr *parse_var(std::istream &in) {
    std::string name;

    while (true) {
        int c = in.peek();
        if (isalpha(c)) {
            consume(in, c);
            name += ((char) c);
        } else
            break;
    }

    if (name.empty())
        throw std::runtime_error("Empty variable name");

    return new VarExpr(name);
}

Expr *parse_let(std::istream &in) {
    parse_char_group(in, "let");

    skip_whitespace(in);

    VarExpr *variable = parse_var(in);

    skip_whitespace(in);

    check_and_consume(in, '=');

    skip_whitespace(in);

    Expr *rhs = parse_expr(in);

    skip_whitespace(in);

    parse_char_group(in, "_in");

    skip_whitespace(in);

    Expr *body = parse_expr(in);

    return new LetExpr(variable, rhs, body);
}

Expr *parse_if(std::istream &in) {
    parse_char_group(in, "if");

    skip_whitespace(in);

    Expr *condition = parse_expr(in);

    skip_whitespace(in);

    parse_char_group(in, "_then");

    skip_whitespace(in);

    Expr *thenBranch = parse_expr(in);

    skip_whitespace(in);

    parse_char_group(in, "_else");

    skip_whitespace(in);

    Expr *elseBranch = parse_expr(in);

    skip_whitespace(in);

    return new IfExpr(condition, thenBranch, elseBranch);
}

Expr *parse_true(std::istream &in) {
    parse_char_group(in, "true");

    skip_whitespace(in);

    return new BoolExpr(true);
}

Expr *parse_false(std::istream &in) {
    parse_char_group(in, "alse");

    skip_whitespace(in);

    return new BoolExpr(false);
}

Expr *parse_fun(std::istream &in) {
    parse_char_group(in, "un");

    skip_whitespace(in);

    parse_char_group(in, "(");
    skip_whitespace(in);

    VarExpr *var = parse_var(in);

    parse_char_group(in, ")");
    skip_whitespace(in);

    Expr *expr = parse_expr(in);
    skip_whitespace(in);

    return new FunExpr(var->to_string(), expr);
}

Expr *parse_underscore(std::istream &in) {
    consume(in, '_');
    int c = in.peek();
    if (c == 'l')
        return parse_let(in);
    else if (c == 't')
        return parse_true(in);
    else if (c == 'i')
        return parse_if(in);
    else if (c == 'f') {
        consume(in, 'f');
        c = in.peek();
        if (c == 'a')
            return parse_false(in);
        else
            return parse_fun(in);
    } else
        throw std::runtime_error("Unknown underscore syntax");
}

Expr *parse_inner(std::istream &in) {
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
        return parse_underscore(in);
    } else {
        consume(in, c);
        throw std::runtime_error("Invalid input!");
    }
}

Expr *parse_multicand(std::istream &in) {
    skip_whitespace(in);

    Expr *expr = parse_inner(in);
    skip_whitespace(in);
    while (in.peek() == '(') {
        consume(in, '(');
        Expr *actual_arg = parse_expr(in);
        skip_whitespace(in);
        consume(in, ')');
        expr = new CallExpr(expr, actual_arg);
    }

    return expr;
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
        return new MultExpr(e, rhs);
    } else
        return e;
}

Expr *parse_comparg(std::istream &in) {
    skip_whitespace(in);

    Expr *e = parse_addend(in);

    skip_whitespace(in);

    int c = in.peek();
    if (c == '+') {
        consume(in, '+');
        Expr *rhs = parse_comparg(in);
        return new AddExpr(e, rhs);
    } else
        return e;
}

Expr *parse_expr(std::istream &in) {
    skip_whitespace(in);

    Expr *e = parse_comparg(in);

    skip_whitespace(in);

    int c = in.peek();
    if (c == '=') {
        parse_char_group(in, "==");
        Expr *rhs = parse_expr(in);
        return new EqualExpr(e, rhs);
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

/***********************************
 ************** HELPERS ************
 ***********************************/

bool check_and_consume(std::istream &in, char expected) {
    if (in.peek() != expected)
        return false;
    else
        consume(in, expected);

    return true;
}

void parse_char_group(std::istream &in, std::string expected) {
    for (std::string::iterator iterator = expected.begin();
         iterator < expected.end(); iterator++) {
        if (!check_and_consume(in, *iterator)) {
            throw std::runtime_error("Unexpected token");
        }
    }
}

void consume(std::istream &in, int expect) {
    int c = in.get();
    assert (c == expect);
}

void skip_whitespace(std::istream &in) {
    while (true) {
        int c = in.peek();
        if (!isspace(c))
            break;
        consume(in, c);
    }
}